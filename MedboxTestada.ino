#include <WiFi.h> // Biblioteca para conectar o esp32 ao wifi
#include <WebServer.h> // Biblioteca para criar um servidor web http no esp32
#include <Preferences.h> // Armaneza dados permanentes na memoria nao volatil do esp32
#include <NTPClient.h> // Obtem a hora atual via NTP (Network time protocol)
#include <WiFiUdp.h> // 
#include <Wire.h> //  COmunicao l c para dispositivos externos
#include <RTClib.h> // interface com relogio em tempo real rtc

// ===== CONSTANTES E DEFINIÇÕES =====
#define NUM_LEDS 7
#define LED_R 25
#define LED_G 26
#define LED_B 27
#define BATERIA_PIN 34
#define PORTA_SERVIDOR 80

// ===== VARIÁVEIS GLOBAIS =====
Preferences prefs;
WebServer server(PORTA_SERVIDOR);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600, 60000); // UTC-3 com atualização a cada 60s
RTC_DS3231 rtc;

const int leds[NUM_LEDS] = {13, 12, 14, 33, 32, 22, 23};
String horarios[NUM_LEDS]; 
bool ledStates[NUM_LEDS] = {false};

// ===== PROTÓTIPOS DE FUNÇÃO =====
void connectWiFi(); //
void initServer(); // Conecta ao wifi usando credenciais proprias
void initRTC();
void handleRoot(); //  Gera pagina HTML com formularios de configuracao
void handleSetWiFi(); // Processa POST para salvar novas credenciais WI-FI
void handleSetHorarios(); // Recebe e valida os horarios para a ativacao do LEDs
void handleGetStatus(); // Retorna JSON com status completo do sistema
void handleNotFound(); // Trata requisicoes para rotas nao existentes
void atualizarRGB(); // Controla LED RGB indicador de bateria e verde (>70%), Amarelo (30-70%), vermelho (<30%)
void checarHorarios(); // Compara hora atual com os horarios programados e ativa LEDs conforme necessarios
void sincronizarRTC(); // Atualiza RTC com a hora do servidor NTP
String getHorarioAtual(); // Retorna string formatada com hora atual RTC 
float getBateriaPercentual(); // Le a tensao do pino analogico e calcula a porcentagem baseada em valores tipicos do LiPO
String formatarTempo(int valor); // Formata Horas/Minutos com dois digitos
// ===== CONFIGURAÇÃO INICIAL =====
void setup() { 
  // Inicializa o Hardware e sistemas, conecta ao wifi, configura o servidor web e sincroniza relogio
  Serial.begin(115200);
  while (!Serial); // Aguarda porta serial no Arduino

  // Inicializar GPIOs
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  connectWiFi();
  initRTC();
  initServer();

  // Inicializar sistemas
  
}
void loop ()
{
  server.handleClient();
  timeClient.update();
  
  static unsigned long ultimaVerificacao = 0;
  if (millis() - ultimaVerificacao >= 30000) { // Verifica a cada 30s
    checarHorarios();
    atualizarRGB();
    ultimaVerificacao = millis();
    
    // Sincroniza RTC periodicamente (a cada 6 horas)
    static unsigned long ultimaSincronizacao = 0;
    if (millis() - ultimaSincronizacao >= 21600000) {
      sincronizarRTC();
      ultimaSincronizacao = millis();
    }
  }
}
void connectWiFi() {
  // Gerencia requisicoes web, atualiza ntp periodicamente, verifica horarios programados, atualiza indicador de bateria
  prefs.begin("wifi", true);
  String ssid = prefs.getString("ssid", "");
  String pass = prefs.getString("pass", "");
  prefs.end();

  if (ssid.isEmpty()) {
    Serial.println("Nenhuma rede Wi-Fi configurada. Use /setWifi para configurar.");
    return;
  }

  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid.c_str(), pass.c_str());
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFalha na conexão Wi-Fi!");
  } else {
    Serial.println("\nConectado com sucesso!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
}
void initRTC() {
  if (!rtc.begin()) {
    Serial.println("RTC não encontrado!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC perdeu energia, configurando tempo padrão");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void initServer() {
  server.on("/", handleRoot);
  server.on("/setWifi", HTTP_POST, handleSetWiFi);
  server.on("/setHorarios", HTTP_POST, handleSetHorarios);
  server.on("/getStatus", handleGetStatus);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}
void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'><title>ESP32 Horários</title>";
  html += "<style>body{font-family:Arial,sans-serif;margin:20px}";
  html += "form{margin-bottom:20px}input{margin:5px 0}button{margin-top:10px}</style></head>";
  html += "<body><h1>Controle de Horários</h1>";
  html += "<h2>Configurar Wi-Fi</h2>";
  html += "<form action='/setWifi' method='post'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Senha: <input type='password' name='pass'><br>";
  html += "<button type='submit'>Salvar</button></form>";
  
  html += "<h2>Configurar Horários</h2>";
  html += "<form action='/setHorarios' method='post'>";
  html += "Horários (HH:MM, separados por vírgula):<br>";
  html += "<input type='text' name='horarios' style='width:300px'><br>";
  html += "<button type='submit'>Salvar</button></form>";
  
  html += "<h2>Status</h2>";
  html += "<p><a href='/getStatus'>Ver status atual</a></p>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}
void handleSetWiFi() {
  if (server.hasArg("ssid") && server.hasArg("pass")) {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");

    prefs.begin("wifi", false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.end();

    server.send(200, "text/plain", "Credenciais Wi-Fi salvas. Reinicie o dispositivo para aplicar.");
  } else {
    server.send(400, "text/plain", "Parâmetros ssid e pass são obrigatórios");
  }
}
void handleSetHorarios() {
  if (server.hasArg("horarios")) {
    String input = server.arg("horarios");
    int count = 0;
    int lastIndex = 0;
    
    for (int i = 0; i <= input.length(); i++) {
      if (i == input.length() || input[i] == ',') {
        String horario = input.substring(lastIndex, i);
        horario.trim();
        
        // Validar formato HH:MM
        if (horario.length() == 5 && horario.indexOf(':') == 2) {
          if (count < NUM_LEDS) {
            horarios[count] = horario;
            count++;
          }
        }
        lastIndex = i + 1;
      }
    }
    
    server.send(200, "application/json", "{\"status\":\"success\",\"message\":\"Horários atualizados\",\"count\":" + String(count) + "}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Parâmetro 'horarios' faltando\"}");
  }
}
void handleGetStatus() {
  DateTime now = rtc.now();
  String status = "{\"horaAtual\":\"" + getHorarioAtual() + "\",";
  status += "\"bateria\":" + String(getBateriaPercentual()) + ",";
  status += "\"wiFi\":\"" + String(WiFi.SSID()) + "\",";
  status += "\"rssi\":" + String(WiFi.RSSI()) + ",";
  status += "\"leds\":[";
  
  for (int i = 0; i < NUM_LEDS; i++) {
    status += "{\"id\":" + String(i) + ",";
    status += "\"horario\":\"" + horarios[i] + "\",";
    status += "\"estado\":" + String(ledStates[i] ? "true" : "false") + "}";
    if (i < NUM_LEDS - 1) status += ",";
  }
  
  status += "]}";
  server.send(200, "application/json", status);
}
void handleNotFound() {
  String message = "Endpoint não encontrado\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
}
void atualizarRGB() {
  float bat = getBateriaPercentual();
  
  if (bat > 70) { // Verde
    analogWrite(LED_R, 0);
    analogWrite(LED_G, 255);
    analogWrite(LED_B, 0);
  } else if (bat > 30) { // Amarelo (vermelho + verde)
    analogWrite(LED_R, 255);
    analogWrite(LED_G, 100);
    analogWrite(LED_B, 0);
  } else { // Vermelho
    analogWrite(LED_R, 255);
    analogWrite(LED_G, 0);
    analogWrite(LED_B, 0);
  }
}
void checarHorarios() {
  DateTime now = rtc.now();
  String horaAtual = formatarTempo(now.hour()) + ":" + formatarTempo(now.minute());
  
  for (int i = 0; i < NUM_LEDS; i++) {
    if (horarios[i].length() == 5 && horarios[i] == horaAtual) {
      digitalWrite(leds[i], HIGH);
      ledStates[i] = true;
    } else {
      digitalWrite(leds[i], LOW);
      ledStates[i] = false;
    }
  }
}
void sincronizarRTC() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.forceUpdate();
    unsigned long epochTime = timeClient.getEpochTime();
    rtc.adjust(DateTime(epochTime));
    Serial.println("RTC sincronizado com NTP");
  }
}
String getHorarioAtual() {
  DateTime now = rtc.now();
  return formatarTempo(now.hour()) + ":" + formatarTempo(now.minute());
}
String formatarTempo(int valor) {
  return (valor < 10) ? "0" + String(valor) : String(valor);
}

float getBateriaPercentual() {
  int leitura = analogRead(BATERIA_PIN);
  float tensao = (leitura / 4095.0) * 3.3 * 1.1; // Considerando divisor de tensão 1:1
  
  // Ajuste para bateria LiPo (3.0V a 4.2V)
  float percentual = (tensao - 3.0) / (4.2 - 3.0) * 100;
  percentual = constrain(percentual, 0, 100);
  
  return percentual;
}



/*
 * ======================================================================
 * CÓDIGO DEFINITIVO PARA O ESP32 DO PROJETO MEDBOX
 * Versão Final: Inclui todas as correções de rede e de compilação.
 * ======================================================================
 * * INSTRUÇÕES PARA QUEM ESTÁ COM O ESP32:
 * * 1. PREENCHA AS 4 SEÇÕES ABAIXO EM "ÁREA DE CONFIGURAÇÃO".
 * * 2. INSTALE a biblioteca "Arduino_JSON" através do Gerenciador de Bibliotecas.
 * * 3. FAÇA O UPLOAD deste código para a sua placa ESP32.
 * * 4. ABRA O MONITOR SERIAL (baud rate 115200) para ver o status.
 * */

// Bibliotecas necessárias
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// ======================================================================
//                 INÍCIO DA ÁREA DE CONFIGURAÇÃO
// ======================================================================

// --- 1. CONFIGURAÇÕES DO WI-FI (PREENCHA COM OS DADOS DA SUA CASA) ---
const char* ssid = "NOME_REDE_WIFI";
const char* password = "SENHA_REDE_WIFI";

// --- 2. CONFIGURAÇÕES DO SERVIDOR (SEU AMIGO VAI TE ENVIAR ESTA URL) ---
// Cole a URL que seu amigo te passar, mantendo o "https://" e o "/led_status" no final.
// Exemplo: "https://a1b2-c3d4.ngrok-free.app/led_status"
String serverUrl = "https://edd738ba01d9.ngrok-free.app/led_status";

// --- 3. MAPEAMENTO DOS PINOS DOS LEDs DOS COMPARTIMENTOS ---
// O primeiro pino (ledPins[0]) corresponde ao Compartimento 1, e assim por diante.
int ledPins[7] = {13, 12, 14, 27, 26, 25, 33}; // <<< VERIFIQUE SE ESTES SÃO OS PINOS CORRETOS

// --- 4. CONFIGURAÇÃO DO STATUS DA BATERIA ---
const int BATTERY_PIN = 34;       // Pino para ler a voltagem da bateria (ADC)
const int RGB_RED_PIN = 21;       // Pino para o componente Vermelho do LED RGB
const int RGB_GREEN_PIN = 19;     // Pino para o componente Verde do LED RGB
const int RGB_BLUE_PIN = 18;      // Pino para o componente Azul do LED RGB

const float VOLTAGE_MAX = 4.2;    // Voltagem da bateria em 100%
const float VOLTAGE_MIN = 3.0;    // Voltagem da bateria em 0%
const float VOLTAGE_DIVIDER_FACTOR = 2.0; // Fator do divisor de tensão (use 1.0 se não houver)

// ======================================================================
//                  FIM DA ÁREA DE CONFIGURAÇÃO
// ======================================================================


// Função para definir a cor do LED RGB usando digitalWrite
void setRgbColor(int red, int green, int blue) {
  // Assume LED de catodo comum (HIGH = LIGA). Para anodo comum, use !red, !green, !blue.
  digitalWrite(RGB_RED_PIN, red);
  digitalWrite(RGB_GREEN_PIN, green);
  digitalWrite(RGB_BLUE_PIN, blue);
}

// Função para ler a bateria e atualizar o LED RGB de status
void updateBatteryStatusLed() {
  int adcValue = analogRead(BATTERY_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;
  float realVoltage = voltage * VOLTAGE_DIVIDER_FACTOR;
  int percentage = map(realVoltage * 100, VOLTAGE_MIN * 100, VOLTAGE_MAX * 100, 0, 100);
  percentage = constrain(percentage, 0, 100);

  Serial.print("[Bateria] Voltagem: ");
  Serial.print(realVoltage);
  Serial.print("V, Porcentagem: ");
  Serial.print(percentage);
  Serial.println("%");

  if (percentage > 70) {
    setRgbColor(LOW, HIGH, LOW); // Verde
  } else if (percentage > 30) {
    setRgbColor(HIGH, HIGH, LOW); // Amarelo
  } else {
    setRgbColor(HIGH, LOW, LOW); // Vermelho
  }
}

// Função para conectar ao Wi-Fi
void connectToWiFi() {
  Serial.print("[WiFi] Conectando a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) {
    delay(500);
    Serial.print(".");
    attempt++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi] Conectado com sucesso!");
    Serial.print("[WiFi] Endereço IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n[WiFi] Falha ao conectar.");
  }
}

// Função para buscar o estado dos LEDs no servidor
void updateLedStatus() {
  HTTPClient http;
  Serial.print("[HTTP] Fazendo requisição para: ");
  Serial.println(serverUrl);

  http.begin(serverUrl);

  // Header para pular a página de aviso do ngrok (corrige o erro 404)
  http.addHeader("ngrok-skip-browser-warning", "true");

  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) { // OK
    String payload = http.getString();
    Serial.print("[HTTP] Resposta recebida: ");
    Serial.println(payload);
    
    JSONVar myObject = JSON.parse(payload);
    
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("[JSON] Erro ao analisar a resposta.");
      http.end();
      return;
    }
    
    // Usa JSONVar para o array (corrige o erro de compilação)
    JSONVar leds = myObject["leds"];
    
    if (leds.length() != 7) {
        Serial.println("[JSON] Erro: O servidor não enviou o status para os 7 LEDs.");
        http.end();
        return;
    }
    Serial.println("[LEDs] Atualizando status...");
    for(int i=0; i<7; i++) {
      int status = (int)leds[i];
      digitalWrite(ledPins[i], status);
    }
  } else {
    Serial.print("[HTTP] Erro na requisição. Código de resposta: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}


void setup() {
  Serial.begin(115200);
  Serial.println("\n[MedBox] Inicializando...");

  // Configura os pinos dos LEDs dos compartimentos como saída
  for(int i=0; i<7; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  Serial.println("[MedBox] Pinos dos LEDs dos compartimentos configurados.");

  // Configuração dos pinos do LED RGB como saída
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  Serial.println("[MedBox] Pinos do LED RGB configurados.");

  // Conecta ao Wi-Fi
  connectToWiFi();
}


void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    updateLedStatus();
    updateBatteryStatusLed();
  } else {
    Serial.println("[Erro] Desconectado do Wi-Fi. Tentando reconectar...");
    setRgbColor(LOW, LOW, HIGH); // Fica azul para indicar que está sem Wi-Fi
    connectToWiFi();
  }
  delay(5000); // Espera 5 segundos antes da próxima verificação
}

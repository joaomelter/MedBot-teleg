#include <Arduino.h>

// Lista de pinos dos 7 LEDs comuns
int leds[] = {13, 12, 14, 33, 32, 22, 23};
int size = sizeof(leds)/sizeof(leds[0]);

#define dt 500 // tempo de delay em ms

// Pinos RGB
const int redPin = 18;   
const int greenPin = 19;  
const int bluePin = 21;   

void setup() {
  // Configura LEDs comuns
  for(int i = 0; i < size; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Configura RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(115200);
  Serial.println("Sistema inicializado!");
}

void loop() {
  // LEDs comuns piscando sequencialmente
  Serial.println("Ligando LEDs sequenciais...");
  for(int i = 0; i < size; i++){
    digitalWrite(leds[i], HIGH);
    delay(dt);
  }

  Serial.println("Desligando LEDs sequenciais...");
  for(int i = size - 1; i >= 0; i--){
    digitalWrite(leds[i], LOW);
    delay(dt);
  }

  // Teste do LED RGB
  Serial.println("Testando RGB...");
  
  digitalWrite(redPin, HIGH); digitalWrite(greenPin, LOW); digitalWrite(bluePin, LOW); // Vermelho
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, LOW); // Verde
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, LOW); digitalWrite(bluePin, HIGH); // Azul
  delay(dt);

  digitalWrite(redPin, HIGH); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, LOW); // Amarelo
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, HIGH); // Ciano
  delay(dt);

  digitalWrite(redPin, HIGH); digitalWrite(greenPin, LOW); digitalWrite(bluePin, HIGH); // Magenta
  delay(dt);

  digitalWrite(redPin, HIGH); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, HIGH); // Branco
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, LOW); digitalWrite(bluePin, LOW); // Desligado
  delay(dt);
}
#include <Arduino.h>

// Lista de pinos dos 7 LEDs comuns
int leds[] = {13, 12, 14, 33, 32, 22, 23};
int size = sizeof(leds)/sizeof(leds[0]);

#define dt 500 // tempo de delay em ms

// Pinos RGB
const int redPin = 18;   
const int greenPin = 19;  
const int bluePin = 21;   

void setup() {
  // Configura LEDs comuns
  for(int i = 0; i < size; i++) {
    pinMode(leds[i], OUTPUT);
  }

  // Configura RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(115200);
  Serial.println("Sistema inicializado!");
}

void loop() {
  // LEDs comuns piscando sequencialmente
  Serial.println("Ligando LEDs sequenciais...");
  for(int i = 0; i < size; i++){
    digitalWrite(leds[i], HIGH);
    delay(dt);
  }

  Serial.println("Desligando LEDs sequenciais...");
  for(int i = size - 1; i >= 0; i--){
    digitalWrite(leds[i], LOW);
    delay(dt);
  }

  // Teste do LED RGB
  Serial.println("Testando RGB...");
  
  digitalWrite(redPin, HIGH); digitalWrite(greenPin, LOW); digitalWrite(bluePin, LOW); // Vermelho
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, LOW); // Verde
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, LOW); digitalWrite(bluePin, HIGH); // Azul
  delay(dt);

  digitalWrite(redPin, HIGH); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, LOW); // Amarelo
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, HIGH); // Ciano
  delay(dt);

  digitalWrite(redPin, HIGH); digitalWrite(greenPin, LOW); digitalWrite(bluePin, HIGH); // Magenta
  delay(dt);

  digitalWrite(redPin, HIGH); digitalWrite(greenPin, HIGH); digitalWrite(bluePin, HIGH); // Branco
  delay(dt);

  digitalWrite(redPin, LOW); digitalWrite(greenPin, LOW); digitalWrite(bluePin, LOW); // Desligado
  delay(dt);
}

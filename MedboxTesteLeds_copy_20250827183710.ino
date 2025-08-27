// Lista de pinos de saída
int leds[] = {13, 12, 14, 33, 32, 22, 23};

int size = sizeof(leds)/sizeof(leds[0]);

#define Led 13
#define dt 500
void setup() {
  // Configura cada pino como saída

  for(int i = 0 ; i < size; i++) pinMode(leds[i], OUTPUT);
}


void loop() {
  // Liga todos os LEDs ao mesmo tempo


  for(int i = 0 ; i <= size ; i++){
    digitalWrite(leds[i], HIGH);
    delay(dt);

    if(i == size){
      for(i = size; i > 0; i-- ){
        digitalWrite(leds[i], LOW);
    delay(dt);
      }
    }
  }


  /*

    digitalWrite(Led, HIGH);
  
  delay(dt); // espera 1s

  // Desliga todos
  
  digitalWrite(Led, LOW);
  
  delay(dt); // espera 1s*/
}

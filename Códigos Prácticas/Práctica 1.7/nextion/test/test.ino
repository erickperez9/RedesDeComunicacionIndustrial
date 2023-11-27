#include <Controllino.h> 

void setup() {
  Serial2.begin(9600);
  pinMode(CONTROLLINO_D0, OUTPUT);
}

void loop() {
  if(Serial2.available()>0){
    String rx_hmi = Serial2.readString();
    if (rx_hmi[0] == 1){
      digitalWrite(CONTROLLINO_D0, HIGH);
    }else if (rx_hmi[0] == 0){
      digitalWrite(CONTROLLINO_D0, LOW);
    }
  }                      
}

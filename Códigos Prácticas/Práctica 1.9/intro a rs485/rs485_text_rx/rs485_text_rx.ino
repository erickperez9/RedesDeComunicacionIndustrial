#include <SPI.h>
#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

/* This function enters loop and waits for any incoming data on serial 3.*/
void RecieveRS485(){
  Serial.println("Recibiendo RS485.");
  while(true){
    if (Serial3.available()) {
      Serial.print((char)Serial3.read()); // print the new byte
    }
  }
}

void setup() {
  /* Here we initialize USB serial at 9600 baudrate for reporting */
  Serial.begin(9600);
  /* Here we initialize RS485 serial at 9600 baudrate for communication */
  Serial3.begin(9600);
  /* This will initialize Controllino RS485 pins */
  Controllino_RS485Init();
}

void loop() {
  RecieveRS485();
}

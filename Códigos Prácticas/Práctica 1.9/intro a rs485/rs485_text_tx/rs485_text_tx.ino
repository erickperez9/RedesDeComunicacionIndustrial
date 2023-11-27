#include <SPI.h>
#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

int retardo = 2000;
void TestRS485 (int com_mode){
  DDRJ = DDRJ | B01100000;
  PORTJ = PORTJ & B10011111;

  switch (com_mode){
    case 0:
      PORTJ = PORTJ & B10011111;
      PORTJ = PORTJ | B01000000;
      delay (10);
      Serial.println ("Enviando mensaje de prueba, no se espera que regrese (SIMPLEX-TX)"); 
      Serial3.println("UUUUU Controllino RS485 test SIMPLEX-TX.UUUUU");
      break;
    
    case 1:
      PORTJ = PORTJ & B10011111;
      PORTJ = PORTJ | B01100000;
      delay (10);
      Serial.println ("Enviando mensaje de prueba, se espera que regrese (SEMI-DUPLEX);");
      Serial3.println("UUUUU Controllino RS485 test SEMI-DUPLEX.UUUUU");
      break;
    
    case 2:
      PORTJ = PORTJ & B10011111;
      PORTJ = PORTJ | B00100000;
      delay (10);
      Serial.println ("Enviando mensaje de prueba, no se espera que se envie (SIMPLEX-RX);"); 
      Serial3.println("UUUUU Controllino RS485 test SIMPLEX-RX.UUUUU");
      break;
    
    default:
      Serial.println("ERROR DE MODO DE COMUNICACION!");
      return; 
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
  /* Send test. 2 seconds apart sending messages with 3 types of different settings. 
  Check the function comment for more infromation */
  TestRS485(0);
  delay(retardo);
  TestRS485(1);
  delay(retardo);
  TestRS485(2);
  delay(retardo);
}

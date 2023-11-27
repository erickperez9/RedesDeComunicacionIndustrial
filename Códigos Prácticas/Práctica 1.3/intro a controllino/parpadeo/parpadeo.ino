#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
void setup() {
  // initialize all used digital output pins as outputs
  DDRE = DDRE | B00100000; //Set the ports PDE5 as output
                           //Ports PDx are disabled
}

// the loop function runs over and over again forever
void loop() {
  int retardo = 500;
  PORTE = PORTE | B00100000;  // Digital output D1 HIGH
  delay(retardo);
  PORTE = PORTE & B00000000;  // Digital output D1 LOW
  delay(retardo);
}

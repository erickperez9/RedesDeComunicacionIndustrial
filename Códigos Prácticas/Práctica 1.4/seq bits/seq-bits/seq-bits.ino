#include <Controllino.h> 
bool bandera=true;
int retardo = 500;

void setup() {
  DDRH = B01111000; 
}

// the loop function runs over and over again forever
void loop() {
  PORTH = B00001000; 
  while(true){  
    delay(retardo);
    if (bandera){
      PORTH = PORTH << 1;
      if(PORTH == B01000000){
        bandera = false;
      }
     }else{
      PORTH = PORTH >> 1;
      if(PORTH == B00001000){
        bandera = true;
      }
      }   
  }
}

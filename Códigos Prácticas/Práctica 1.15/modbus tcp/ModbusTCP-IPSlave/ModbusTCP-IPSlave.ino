#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>
#include "Mudbus.h"

Mudbus Mb;
int myID = 1;
//Function codes 1(read coils), 3(read registers), 5(write coil), 6(write register)
//signed int Mb.R[0 to 125] and bool Mb.C[0 to 128] MB_N_R MB_N_C
//Port 502 (defined in Mudbus.h) MB_PORT
void setup(){
  uint8_t mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };
  uint8_t ip[] = { 192, 168, 1, 101 }; // CHANGE this to the IP address of your CONTROLLINO
  uint8_t gateway[] = { 192, 168, 0, 254 };
  uint8_t subnet[] = { 255, 255, 255, 0 };
  Ethernet.begin(mac, ip, gateway, subnet);
  delay(5000);
  Serial.begin(9600);
  Serial.println("Started");
  pinMode(CONTROLLINO_D0, OUTPUT); 
}
void loop(){ 
  Mb.Run();
  if(Mb.idslave == myID){
    digitalWrite(CONTROLLINO_D0, Mb.R[2]);
  }
  Serial.println(Mb.idslave);
}

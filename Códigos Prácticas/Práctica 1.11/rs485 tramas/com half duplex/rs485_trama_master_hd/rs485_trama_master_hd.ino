#include <SPI.h>
#include <Controllino.h>
 
#define HEAD 0xAA
#define TAIL 0xFE
 
// Comandos 
#define CMD_ON      0x01
#define CMD_OFF     0x02
#define CMD_READ_1   0x03
#define CMD_READ_2  0x04
  
#define SLAVE_ID 0x23
byte trama_tx[4], trama_rx[4], idx;
 
void txFrame(byte esclavo, byte cmd){
  trama_tx[1] = esclavo;
  trama_tx[2] = cmd;
  Controllino_RS485TxEnable();
  Serial3.write(trama_tx, 4);
  Serial3.flush();  
}
 
 
int rxFrame( byte esclavo ){
  Controllino_RS485RxEnable();
  Serial3.readBytes( trama_rx, 4 );
  if( trama_rx[0] != HEAD )    // error en la trama rx
    return 0;
    
  if( trama_rx[1] != esclavo ) // rx de otro esclavo
    return 0;
 
  if( trama_rx[3] != TAIL )    // error en trama rx
    return 0;
  
  return trama_rx[2];
}
 
 
void setup() {
  Serial.begin(19200);
  Controllino_RS485Init(9600);
  Controllino_RS485RxEnable();
  trama_tx[0] = HEAD;
  trama_tx[3] = TAIL;
}
 
 
void loop() {
  
  if( Serial.available() ){
    delay(500);
    char c = Serial.read();
    int read_1, read_2;
    switch(c){
      case '0':
        txFrame(SLAVE_ID, CMD_ON);
        break;
        
      case '1':
        txFrame(SLAVE_ID, CMD_OFF);
        break;
 
      case '2':
        txFrame(SLAVE_ID, CMD_READ_1);
        Serial.print("Read 1: ");
        read_1 = rxFrame(SLAVE_ID);
        if( read_1 == 0 )
          Serial.println( "No frame RX" );
        else
          Serial.println(read_1, DEC);
        break;
 
      case '3':
        txFrame(SLAVE_ID, CMD_READ_2);
        Serial.print("Read 2: ");
        read_2 = rxFrame(SLAVE_ID);
        if( read_2 == 0 )
          Serial.println( "No frame RX" );
        else
          Serial.println(read_2, DEC);
        break;
 
      default:
        break;
    }
  }
}

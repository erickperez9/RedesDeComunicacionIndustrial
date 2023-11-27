#include <SPI.h>
#include <Controllino.h>

#define HEAD 0xAA
#define MY_ID 0x23
#define TAIL 0xFE
 
//Comandos 
#define CMD_ON      0x01
#define CMD_OFF     0x02
#define CMD_READ_1   0x03
#define CMD_READ_2  0x04
  
byte trama_rx[5], trama_tx[5], idx;
float read_1, read_2;
 
void txFrame( float x ){
  trama_tx[2] = (byte)x;  
  Controllino_RS485TxEnable();
  Serial3.write(trama_tx, 4);
  Serial3.flush(); // wait until the trasmission is complete
  Controllino_RS485RxEnable(); 
}
 
void ejecutarComando(){
  Serial.println("Ejectutando comando!!!");
  if ( trama_rx[1] != MY_ID ) // el mensaje es para otro esclavo
    return;
 
  switch( trama_rx[2] ){  // ejecutar comando
 
    case CMD_ON:                      
      digitalWrite( CONTROLLINO_D1, HIGH );  
      break;
 
    case CMD_OFF:                     
      digitalWrite( CONTROLLINO_D1, LOW );
      break;
    
    case CMD_READ_1:
      read_1 = 12;     
      txFrame( read_1 );
      break;
 
    case CMD_READ_2:
      read_2 = 50;
      txFrame( read_2 );
      break;
 
    default:                             
      break;
  }
}
 
 
void setup() {
  Serial.begin(19200);
  Controllino_RS485Init(9600);
  Controllino_RS485RxEnable();
  
  pinMode( CONTROLLINO_D1, OUTPUT ); 
  digitalWrite( CONTROLLINO_D1, LOW );  

  trama_tx[0] = HEAD;
  trama_tx[1] = MY_ID;
  trama_tx[3] = TAIL;
  idx = 0;
}
 
 
void loop() {
  if (Serial3.available() > 0){
    byte incoming =  Serial3.read();
    Serial.print("Recibido: ");
    Serial.println(incoming);
      
    if( idx == 0 ){           // principio de trama
      if( incoming != HEAD ) 
        return;
   
      trama_rx[idx] = incoming;
      idx++;
    }
    else if ( idx > 0 && idx < 4 ){ 
      trama_rx[idx++] = incoming;     
       
      if ( idx == 4 ){                // fin de trama
        if( trama_rx[3] == TAIL )         
          ejecutarComando();
        idx = 0;
      }
    }
  }
}

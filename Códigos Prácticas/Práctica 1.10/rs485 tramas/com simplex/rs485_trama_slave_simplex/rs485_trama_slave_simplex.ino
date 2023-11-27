#include <SPI.h>
#include <Controllino.h>
 
#define INICIO 0xAA
#define MY_ID 0x23
#define FIN 0xFE
#define CMD_ON 0x01
#define CMD_OFF 0x02
 
byte trama[4], idx;
 
void setup() {
  // Configurar Serial a 9600 baudios
  Serial.begin(9600);
  Controllino_RS485Init(9600);
  Controllino_RS485RxEnable();

  Serial.println("Listo para RX COM RS485...");
  pinMode( CONTROLLINO_D1, OUTPUT );
  digitalWrite( CONTROLLINO_D1, LOW );
 
  idx = 0;
}
 
void ejecutarComando(){
  Serial.println("Ejectutando comando!!!");
  if ( trama[1] != MY_ID ) // el mensaje es para otro esclavo
    return;
 
  switch( trama[2] ){                      // ejecutar comando
 
    case CMD_ON:                      
      digitalWrite( CONTROLLINO_D1, HIGH );  
      break;
 
    case CMD_OFF:                     
      digitalWrite( CONTROLLINO_D1, LOW );
      break;
 
    default:                              
      break;
  }
}
 
void loop() {
  if (Serial3.available() > 0){
    byte byte_rx = Serial3.read();
    Serial.print("Recibido: ");
    Serial.println(byte_rx, HEX);
      
    if( idx == 0 ){           // principio de trama
      if( byte_rx != INICIO ) // trama incorrecta
        return;
   
      trama[idx] = byte_rx;
      idx++;
    }
    else if ( idx > 0 && idx < 4 ){ // 
      trama[idx++] = byte_rx; // va creando la trama con los bytes rx
       
      if ( idx == 4 ){                // fin de trama
        if( trama[3] == FIN )         
          ejecutarComando();
        idx = 0;
      }
    }
  } 
}

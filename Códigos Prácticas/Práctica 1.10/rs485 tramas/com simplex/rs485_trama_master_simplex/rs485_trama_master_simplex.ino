#include <Controllino.h>
 
#define INICIO 0xAA
#define FIN 0xFE
#define CMD_ON 0x01
#define CMD_OFF 0x02
#define SLAVE_ID 0x23  
 
byte frame[4], idx;
char cmd;

void setup() {
  Serial.begin(19200); 
  
  pinMode(CONTROLLINO_D0, OUTPUT);
  digitalWrite(CONTROLLINO_D0, LOW);

  Controllino_RS485Init(9600);
  Controllino_RS485TxEnable();

  cmd = '0';
  frame[0] = INICIO;
  frame[1] = SLAVE_ID;        
  frame[3] = FIN;
}
 
 
void loop() {
  if( Serial.available() ){
    delay(1000);
    digitalWrite( CONTROLLINO_D0, HIGH );
 
    cmd = Serial.read();
    Serial.print("Comando a ejecutar en esclavo: ");
    Serial.println(cmd);
    
    if( cmd == '0' ){
      Serial.println("Apagando");
      frame[2] = CMD_OFF; // apagar led
      Serial3.write( frame, 4 );
      
      Serial.println( frame[0], HEX );
      Serial.println( frame[1], HEX );
      Serial.println( frame[2], HEX );
      Serial.println( frame[3], HEX );
      Serial.println( );
    }
    else if ( cmd == '1' ){
      Serial.println("Encendiendo");     
      frame[2] = CMD_ON; 
      Serial3.write( frame, 4 );
      
      Serial.println( frame[0], HEX );
      Serial.println( frame[1], HEX );
      Serial.println( frame[2], HEX );
      Serial.println( frame[3], HEX );
      Serial.println( );
 
    }   
  }
  delay(1000);   
  digitalWrite( CONTROLLINO_D0, LOW);
}

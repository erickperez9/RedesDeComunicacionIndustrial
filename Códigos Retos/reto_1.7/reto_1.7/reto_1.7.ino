#include <Controllino.h>  // libreria para controllino
#include <SPI.h>          // libreria para SPI
#include <DHT_U.h>        // libreria para sensores DHT
#include <DHT.h>          // libreria para sensores DHT
#include "Nextion.h"      // libreria para NEXTION

#define DHTTYPE DHT11     // tipo de sensor DHT
#define DHTPIN 42         // pin de conexion sensor DHT 11

#define TH_T 28           // threshold de temperatura
#define TH_H 60           // threshold de humedad

DHT dht(DHTPIN, DHTTYPE); // objeto DHT

              // objetoNextion(pagina, id, nombre))
NexWaveform s0 = NexWaveform(0, 1, "s0"); // waveform NEXTION
NexNumber n0 = NexNumber(0, 2, "n0");     // number NEXION 
NexNumber n1 = NexNumber(0, 5, "n1");     // number NEXION
NexButton b0 = NexButton(1, 1, "b0");     // boton NEXION
NexButton b1 = NexButton(1, 2, "b1");     // boton NEXION
NexButton b2 = NexButton(1, 5, "b2");     // boton NEXION
NexButton b3 = NexButton(1, 6, "b3");     // boton NEXION
NexButton b4 = NexButton(1, 8, "b4");     // boton NEXION
NexButton b5 = NexButton(1, 9, "b5");     // boton NEXION
NexButton b6 = NexButton(1, 11, "b6");    // boton NEXION
NexButton b7 = NexButton(1, 12, "b7");    // boton NEXION
NexButton b8 = NexButton(1, 14, "b8");    // boton NEXION
NexButton b9 = NexButton(1, 15, "b9");    // boton NEXION
NexText t8 = NexText(0, 13, "t8");        // texto NEXION
NexText t9 = NexText(0, 14, "t9");        // texto NEXION

NexTouch *nex_listen_list[] = {&b0,&b1,&b2,&b3,&b4,&b5,&b6,&b7,&b8,&b9,NULL};

//FUNCIONES DE ENCENDIDO Y APAGADO PARA DIRIGIR A LOS BOTONES
void ON_D0(){digitalWrite(CONTROLLINO_D0,1);}
void OFF_D0(){digitalWrite(CONTROLLINO_D0,0);}
void ON_D1(){digitalWrite(CONTROLLINO_D1,1);}
void OFF_D1(){digitalWrite(CONTROLLINO_D1,0);}
void ON_D2(){digitalWrite(CONTROLLINO_D2,1);}
void OFF_D2(){digitalWrite(CONTROLLINO_D2,0);}
void ON_D3(){digitalWrite(CONTROLLINO_D3,1);}
void OFF_D3(){digitalWrite(CONTROLLINO_D3,0);}
void ON_D4(){digitalWrite(CONTROLLINO_D4,1);}
void OFF_D4(){digitalWrite(CONTROLLINO_D4,0);}

void setup() {
  nexInit();                        //inicia NEXTION LIBRARY
  delay(10);
  pinMode(CONTROLLINO_D0, OUTPUT);
  pinMode(CONTROLLINO_D1, OUTPUT);
  pinMode(CONTROLLINO_D2, OUTPUT);
  pinMode(CONTROLLINO_D3, OUTPUT);
  pinMode(CONTROLLINO_D4, OUTPUT);
  pinMode(CONTROLLINO_R0, OUTPUT);
  pinMode(CONTROLLINO_R1, OUTPUT);
  b0.attachPush(ON_D0); delay(10);             // accion de boton b0
  b1.attachPush(OFF_D0);delay(10);
  b2.attachPush(ON_D1);delay(10);
  b3.attachPush(OFF_D1);delay(10);
  b4.attachPush(ON_D2);delay(10);
  b5.attachPush(OFF_D2);delay(10);
  b6.attachPush(ON_D3);delay(10);
  b7.attachPush(OFF_D3);delay(10);
  b8.attachPush(ON_D4);delay(10);
  b9.attachPush(OFF_D4);delay(10);
 
  dht.begin();
  delay(10);
}

void loop() {         
  delay(50);
  int h = dht.readHumidity();     // lee temperatura del sensor
  int t = dht.readTemperature();  // lee humedad del sensor
  delay(10);
  nexLoop(nex_listen_list);       // loop para botones
  
  // ******************************************************************************
  //   CREAR LOGICA PARA REDUCIR EL DELAY DE LA COMUNICACION (USAR FUNCION MILLIS)
  // ******************************************************************************
  
  s0.addValue(0, h);
  n1.setValue(h);
  s0.addValue(1, t);  
  n0.setValue(t); 
    
  if( t >= TH_T ){
    digitalWrite(CONTROLLINO_R0, HIGH); //RELE DE SISTEMA DE REFIGERACION ACTIVADO
    t8.setText("ON");
  }else{
    digitalWrite(CONTROLLINO_R0, LOW);  //RELE DE SISTEMA DE REFIGERACION DESACTIVADO
    t8.setText("OFF");
  }
  if( h <= TH_H ){
    digitalWrite(CONTROLLINO_R1, HIGH); //RELE DE SISTEMA DE HUMIDIFICACION ACTIVADO
    t9.setText("ON"); 
  }else{
    digitalWrite(CONTROLLINO_R1, LOW);  //RELE DE SISTEMA DE HUMIDIFICACION DESACTIVADO
    t9.setText("OFF"); 
  }
}

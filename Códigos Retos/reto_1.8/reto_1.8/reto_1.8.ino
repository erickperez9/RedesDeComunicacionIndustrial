#include <Controllino.h>  // libreria para controllino
#include <SPI.h>          // libreria para SPI
#include <DHT_U.h>        // libreria para sensores DHT
#include <DHT.h>          // libreria para sensores DHT
#include "Nextion.h"      // libreria para NEXTION
#include <Ethernet.h>     // libreria para modulo ETH
#include <PubSubClient.h> // libreria para com MQTT

#define DHTTYPE DHT11     // tipo de sensor DHT
#define DHTPIN 42         // pin de conexion sensor DHT 11

#define TH_T 28           // threshold de temperatura
#define TH_H 60           // threshold de humedad

// CREDENCIALES MQTT
#define MQTT_SERVER   "demo.thingsboard.io"
#define MQTT_USERNAME "testid3"
#define MQTT_PORT     1883
#define MQTT_PASSWORD "testpass3"         
#define MQTT_NAME     "testname3"

//CONFIGURACION ETHERNET
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 18, 101);
IPAddress myDns(8, 8, 8, 8);
EthernetClient client;
PubSubClient mqtt(client);

DHT dht(DHTPIN, DHTTYPE); // objeto DHT

//objetoNextion nombre = objetoNextion(pagina, id, nombre))
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
  Serial.begin(9600);
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  delay(10); 
  mqtt.setServer(MQTT_SERVER, MQTT_PORT); 
  delay(10);
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

  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
    } else {
      Serial.println("failed");
      delay(5000);
    }
  } else {
    mqtt.loop();
    String dataJS = "{\"temperature\":" + String(t) + ",\"humidity\":" +String(h) + "}";
    char json[100];
    dataJS.toCharArray(json,dataJS.length()+1);
    mqtt.publish("v1/devices/me/telemetry", json);
    delay(100);
  }
}

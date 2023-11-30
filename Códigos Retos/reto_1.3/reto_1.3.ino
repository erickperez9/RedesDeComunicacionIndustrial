#include <Controllino.h>  // libreria para controllino
#include <DHT_U.h>        // libreria para sensores DHT
#include <DHT.h>          //  libreria para sensores DHT

#define DHTTYPE DHT11     // tipo de sensor DHT
#define DHTPIN 42         // pin de conexion sensor DHT 11

#define TH_T 20           // threshold de temperatura
#define TH_H 50           // threshold de humedad

DHT dht(DHTPIN, DHTTYPE); // objeto DHT

// SETUP RELES COMO SALIDAS E INICIA SENSOR DHT
void setup() {
  Serial.begin(9600);
  pinMode(CONTROLLINO_R0, OUTPUT);
  pinMode(CONTROLLINO_R1, OUTPUT);
  dht.begin();
}

// LOOP OBTIENE DATOS DEL SENSOR Y COMPARA CON LO TH Y ACCIONA LOS RELES
void loop() {
  delay(1000);
  int h = dht.readHumidity();     // lee temperatura del sensor 
  int t = dht.readTemperature();  // lee humedad del sensor
  Serial.print("TEMPERATURA = "); Serial.print(t);
  Serial.print("\tHUMEDAD = ");   Serial.println(h);
  
  if( t > TH_T ){
    digitalWrite(CONTROLLINO_R0, HIGH);
    Serial.println("RELE DE SISTEMA DE REFIGERACION ACTIVADO");
  }else{
    digitalWrite(CONTROLLINO_R0, LOW);
    Serial.println("RELE DE SISTEMA DE REFIGERACION DESACTIVADO");
  }

  if( h < TH_H ){
    digitalWrite(CONTROLLINO_R1, HIGH);
    Serial.println("RELE DE SISTEMA DE HUMIDIFICACION ACTIVADO");
  }else{
    digitalWrite(CONTROLLINO_R1, LOW);
    Serial.println("RELE DE SISTEMA DE REFIGERACION DESACTIVADO");
  }
  Serial.println("");
}

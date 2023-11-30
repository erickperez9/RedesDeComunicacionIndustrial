#include <Controllino.h>
#include <DHT_U.h>

#define DHTTYPE DHT11
#define DHTPIN 42

#define TH_T 25
#define TH_H 60

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  DDRA = DDRA | B00000011;
  PORTA = PORTA & B11111100;  // Inicia los reles en estado LOW
  dht.begin();
}

void loop() {
  delay(1000);
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  Serial.print("TEMPERATURA = "); Serial.print(t);
  Serial.print("\tHUMEDAD = "); Serial.println(h);

  if (t > TH_T && h < TH_H) {
    PORTA = PORTA | B00000011;               // RELES R0 y R1 HIGH
    Serial.println("RELE DE SISTEMA DE REFRIGERACION ACTIVADO");
    Serial.println("RELE DE SISTEMA DE HUMIDIFICACION ACTIVADO");
  }
  else if (t > TH_T && h >= TH_H) {
    PORTA = (PORTA | B00000001) & B11111101; // RELE R0 HIGH
    Serial.println("RELE DE SISTEMA DE REFRIGERACION ACTIVADO");
    Serial.println("RELE DE SISTEMA DE HUMIDIFICACION DESACTIVADO");
  }
  else if (t <= TH_T && h < TH_H) {          // RELE R1 HIGH
    PORTA = (PORTA | B00000010) & B11111110;
    Serial.println("RELE DE SISTEMA DE REFRIGERACION DESACTIVADO");
    Serial.println("RELE DE SISTEMA DE HUMIDIFICACION ACTIVADO");
  }
  else if (t <= TH_T && h >= TH_H) {         // RELES R0 y R1 LOW
    PORTA = PORTA & B11111100;
    Serial.println("RELE DE SISTEMA DE REFRIGERACION DESACTIVADO");
    Serial.println("RELE DE SISTEMA DE HUMIDIFICACION DESACTIVADO");
  }

  Serial.println("");
}

#include <Controllino.h>  // libreria para controllino
#include <DHT_U.h>        // libreria para sensores DHT
#include <DHT.h>          // libreria para sensores DHT
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT11     // tipo de sensor DHT
#define DHTPIN 42         // pin de conexion sensor DHT 11

#define TH_T 30           // threshold de temperatura
#define TH_H 50           // threshold de humedad

DHT dht(DHTPIN, DHTTYPE); // objeto DHT
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// SETUP RELES COMO SALIDAS E INICIA SENSOR DHT Y LCD
void setup() {
  Serial.begin(9600);
  pinMode(CONTROLLINO_R0, OUTPUT);
  pinMode(CONTROLLINO_R1, OUTPUT);
  dht.begin();
  lcd.init(); // initialize the lcd                      
  // Print a message to the LCD.
  lcd.backlight(); 
}

// LOOP OBTIENE DATOS DEL SENSOR Y COMPARA CON LO TH Y ACCIONA LOS RELES
void loop() {
  delay(1000);
  int h = dht.readHumidity();     // lee temperatura del sensor 
  int t = dht.readTemperature();  // lee humedad del sensor
  Serial.print("TEMPERATURA = "); Serial.print(t);
  Serial.print("\tHUMEDAD = ");   Serial.println(h);

  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(t);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(9, 0);
  lcd.print("R0: ");
  lcd.setCursor(9, 1);
  lcd.print("R1: ");
  
  if( t >= TH_T ){
    digitalWrite(CONTROLLINO_R0, HIGH);
    Serial.println("RELE DE SISTEMA DE REFIGERACION ACTIVADO");
    lcd.setCursor(13, 0);
    lcd.print("ON ");
  }else{
    digitalWrite(CONTROLLINO_R0, LOW);
    Serial.println("RELE DE SISTEMA DE REFIGERACION DESACTIVADO");
    lcd.setCursor(13, 0);
    lcd.print("OFF");
  }

  if( h <= TH_H ){
    digitalWrite(CONTROLLINO_R1, HIGH);
    Serial.println("RELE DE SISTEMA DE HUMIDIFICACION ACTIVADO");
    lcd.setCursor(13, 1);
    lcd.print("ON ");
  }else{
    digitalWrite(CONTROLLINO_R1, LOW);
    Serial.println("RELE DE SISTEMA DE REFIGERACION DESACTIVADO");
    lcd.setCursor(13, 1);
    lcd.print("OFF");
  }
  Serial.println("");
}

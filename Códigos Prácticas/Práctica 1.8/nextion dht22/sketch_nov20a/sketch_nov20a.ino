#include <Controllino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include "Nextion.h"
#include "DHT.h"

#define DHTPIN 42
#define DHTTYPE DHT11

#define MQTT_SERVER   "demo.thingsboard.io"
#define MQTT_USERNAME "XjeeIv4h9FNuc6BkkjZs"
#define MQTT_PORT     1883
#define MQTT_PASSWORD ""         
#define MQTT_NAME     "mqtt-test"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 18, 101);
IPAddress myDns(8, 8, 8, 8);
EthernetClient client;
PubSubClient mqtt(client);

DHT dht(DHTPIN, DHTTYPE);
NexWaveform s0 = NexWaveform(0, 1, "s0");
NexNumber n0 = NexNumber(0, 1, "n0");
NexNumber n1 = NexNumber(0, 1, "n1");

void setup(void){
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
  dht.begin();
  delay(10);
  nexInit();
}

void loop(void){
  delay(50);
  int h = dht.readHumidity();
  int t = dht.readTemperature();
 
  s0.addValue(0, h);
  n1.setValue(h);
  s0.addValue(1, t);  
  n0.setValue(t);  

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
    delay(1000);
  }
  
}

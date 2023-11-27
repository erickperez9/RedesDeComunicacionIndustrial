#include <Ethernet.h>
#include <ModbusTCPMaster.h>

// Ethernet configuration values
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 3);
IPAddress slaveIp(192, 168, 1, 100);
const uint16_t slavePort = 502;

// Define the ModbusTCPMaster object
ModbusTCPMaster modbus;

// Ethernet client object used to connect to the slave
EthernetClient slave;

uint32_t lastSentTime = 0UL;

void setup() {
  Serial.begin(9600UL);

  // Begin Ethernet
  Ethernet.begin(mac, ip);
  Serial.println(Ethernet.localIP());

  // NOTE: it is not necessary to start the modbus master object
}


void loop() {
  // Connect to slave if not connected
  // The ethernet connection is managed by the application, not by the library
  // In this case the connection is opened once
  if (!slave.connected()) {
    slave.stop();

    slave.connect(slaveIp, slavePort);
    if (slave.connected()) {
      Serial.println("Reconnected");
    }
  }

  // Send a request every 1000ms if connected to slave
  if (slave.connected()) {
    if (millis() - lastSentTime > 1000) {
      // Set random values
      bool values[5];
      for (int i = 0; i < 5; ++i) {
        values[i] = random() & 0x01;
      }

      // Send a Write Multiple Coils request to the slave with address 31
      // It requests for setting 5 coils starting in address 0
      // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
      // blocking, so you can continue the program while the Modbus functions work. To check for
      // available responses, call modbus.available() function often.
      if (!modbus.writeMultipleCoils(slave, 31, 0, values, 5)) {
        // Failure treatment
        Serial.println("Request fail");
      }

      lastSentTime = millis();
    }

    // Check available responses often
    if (modbus.isWaitingResponse()) {
      ModbusResponse response = modbus.available();
      if (response) {
        if (response.hasError()) {
          // Response failure treatment. You can use response.getErrorCode()
          // to get the error code.
          Serial.print("Error ");
          Serial.println(response.getErrorCode());
        } else {
          Serial.println("Done");
        }
      }
    }
  }
}
#include <ModbusRTUMaster.h>
#include <ModbusTCPMaster.h>
#include <RS485.h>
#include <RS232.h>
#include <Ethernet.h>

// Ethernet configuration values
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 3);
IPAddress slaveIp(192, 168, 1, 100);
const uint16_t slavePort = 502;

ModbusRTUMaster master(RS485);
ModbusRTUMaster master2(RS232);
ModbusTCPMaster modbus;

EthernetClient slave;

#define REGISTERS_TO_WRITE 4
#define COILS_TO_WRITE 5
#define COILS_TO_WRITE_TCP 5

uint32_t lastSentTime = 0UL;
const uint32_t baudrate = 38400UL;
const uint32_t baudrate2 = 19200UL;

void setup() {
  Serial.begin(9600UL);

  Ethernet.begin(mac, ip);
  Serial.println(Ethernet.localIP());

  RS485.begin(baudrate, HALFDUPLEX, SERIAL_8E1);
  RS232.begin(baudrate2, SERIAL_8E1);

  master.begin(baudrate);
  master2.begin(baudrate2);
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
      uint16_t values[REGISTERS_TO_WRITE];
      for (int i = 0; i < REGISTERS_TO_WRITE; ++i) {
        values[i] = random(0, 2);
      }

      bool values2[COILS_TO_WRITE];
      for (int i = 0; i < COILS_TO_WRITE; ++i) {
        values2[i] = random() & 0x01;
      }

      bool values3[5];
      for (int i = 0; i < COILS_TO_WRITE_TCP; ++i) {
        values3[i] = random() & 0x01;
      }

      // Send a Write Multiple Coils request to the slave with address 31
      // It requests for setting 5 coils starting in address 0
      // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
      // blocking, so you can continue the program while the Modbus functions work. To check for
      // available responses, call modbus.available() function often.

      if (!master.writeMultipleRegisters(1, 4, values, REGISTERS_TO_WRITE)) {
        Serial.println("Request fail RS485");
      }
      if (!master2.writeMultipleCoils(31, 0, values2, COILS_TO_WRITE)) {
        Serial.println("Request fail RS232");
      }
      if (!modbus.writeMultipleCoils(slave, 15, 0, values3, COILS_TO_WRITE_TCP)) {
        Serial.println("Request fail TCP");
      }

    lastSentTime = millis();
    }


// Check available responses often
    if (master.isWaitingResponse()) {
      ModbusResponse response = master.available();
      if (response) {
        if (response.hasError()) {
          Serial.print("Error RS485");
          Serial.println(response.getErrorCode());
        } else {
          if (response.hasError()) {
            Serial.print("Error RS485");
            Serial.println(response.getErrorCode());
          } else {
            Serial.println("Done RS485");
          }
        }
      }
    }

    if (master2.isWaitingResponse()) {
      ModbusResponse response2 = master2.available();
      if (response2) {
        if (response2.hasError()) {
          Serial.print("Error RS232");
          Serial.println(response2.getErrorCode());
        } else {
          if (response2.hasError()) {
            Serial.print("Error RS232");
            Serial.println(response2.getErrorCode());
          } else {
            Serial.println("Done RS232");
          }
        }
      }
    }

    // Check available responses often
    if (modbus.isWaitingResponse()) {
      ModbusResponse response3 = modbus.available();
      if (response3) {
        if (response3.hasError()) {
          // Response failure treatment. You can use response.getErrorCode()
          // to get the error code.
          Serial.print("Error TCP");
          Serial.println(response3.getErrorCode());
        } else {
          Serial.println("Done TCP");
        }
      }
    }


  } 
}
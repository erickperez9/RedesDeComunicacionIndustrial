#include <ModbusRTUMaster.h>
#include <RS485.h>

ModbusRTUMaster master(RS485);

#define COILS_TO_WRITE 5
uint32_t lastSentTime = 0UL;
const uint32_t baudrate = 38400UL;

void setup() {
  Serial.begin(9600UL);
  RS485.begin(baudrate, HALFDUPLEX, SERIAL_8E1);
  master.begin(baudrate);
}

void loop() {
  // Send a request every 1000ms
  if (millis() - lastSentTime > 1000) {
    
    // Set random values
    bool values[5];
    for (int i = 0; i < 5; ++i) {
      values[i] = random() & 0x01;
    }

    // Send a Write Multiple Coils request to the slave with address 31
    // It requests for setting COILS_TO_WRITE coils starting in address 0
    // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
    // blocking, so you can continue the program while the Modbus functions work. To check for
    // available responses, call modbus.available() function often.
    if (!master.writeMultipleCoils(31, 0, values, COILS_TO_WRITE)) {
      // Failure treatment
      Serial.println("Request fail");
    }

    lastSentTime = millis();
  }

  // Check available responses often
  if (master.isWaitingResponse()) {
    ModbusResponse response = master.available();
    if (response) {
      if (response.hasError()) {
        // Response failure treatment. You can use response.getErrorCode()
        // to get the error code.
        Serial.print("Error ");
        Serial.println(response.getErrorCode());
      } else {
        // Get the discrete inputs values from the response
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
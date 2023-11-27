#include <ModbusRTUMaster.h>
#include <RS485.h>

ModbusRTUMaster master(RS485);

#define REGISTERS_TO_WRITE 4
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
    uint16_t values[4];
    for (int i = 0; i < REGISTERS_TO_WRITE; ++i) {
      values[i] = random(0, 2);
    }

    // Send a Write Multiple Register request to the slave with address 31
    // It requests for setting REGISTERS_TO_WRITE registers with random values between 0 and 256 starting in address 0
    // IMPORTANT: all read and write functions start a Modbus transmission, but they are not
    // blocking, so you can continue the program while the Modbus functions work. To check for
    // available responses, call modbus.available() function often.
    if (!master.writeMultipleRegisters(1, 4, values, REGISTERS_TO_WRITE)) {
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
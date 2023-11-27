#include <RS485.h>
#include <ModbusRTUSlave.h>

#define RS485_RATE 38400UL

// Modbus registers mapping
// This example uses the M-Duino 21+ mapping
int digitalOutputsPins[] = {  Q0_0, Q0_1, Q0_2, Q0_3, Q0_4};

#define numDigitalOutputs int(sizeof(digitalOutputsPins) / sizeof(int))

bool digitalOutputs[numDigitalOutputs];

// Define the ModbusRTUSlave object with Modbus RTU slave address: 31,
// using the RS-485, RS-232 or Serial1 port, depending on availability

ModbusRTUSlave modbus(RS485, 31);

const uint32_t baudrate = 38400UL;

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600UL);

  // Init variables, inputs and outputs
  for (int i = 0; i < numDigitalOutputs; ++i) {
    digitalOutputs[i] = false;
    digitalWrite(digitalOutputsPins[i], digitalOutputs[i]);
  }

  // Start the serial port
  RS485.begin(baudrate, HALFDUPLEX, SERIAL_8E1);

  // Init ModbusRTUSlave object
  modbus.begin(RS485_RATE);

  modbus.setCoils(digitalOutputs, numDigitalOutputs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

  // Process modbus requests
  modbus.update();

  // Update outputs
  for (int i = 0; i < numDigitalOutputs; ++i) {
    digitalWrite(digitalOutputsPins[i], digitalOutputs[i]);
  }

}


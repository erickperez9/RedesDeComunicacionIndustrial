#include <Ethernet.h>
#include <ModbusTCPSlave.h>

// Ethernet configuration values
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };
IPAddress ip(192, 168, 1, 3);
const uint16_t port = 502;

// Modbus registers mapping
// This example uses the M-Duino21+ mapping
int digitalOutputsPins[] = {
  Q0_0, Q0_1, Q0_2, Q0_3, Q0_4,
};

#define numDigitalOutputs int(sizeof(digitalOutputsPins) / sizeof(int))

bool digitalOutputs[numDigitalOutputs];

// Define the ModbusTCPSlave object
ModbusTCPSlave modbus(port);

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600UL);

  // Init variables, inputs and outputs
  for (int i = 0; i < numDigitalOutputs; ++i) {
    digitalOutputs[i] = false;
    digitalWrite(digitalOutputsPins[i], digitalOutputs[i]);
  }

  // Init Ethernet
  Ethernet.begin(mac, ip);
  Serial.println(Ethernet.localIP());

  // Init ModbusTCPSlave object
  modbus.begin();

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

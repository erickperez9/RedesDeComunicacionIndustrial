#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "ModbusRtu.h"    /* Usage of ModBusRtu library allows you to implement the Modbus RTU protocol in your sketch. */


#define SlaveModbusAdd  1
#define RS485Serial     3

Modbus ControllinoModbusSlave(SlaveModbusAdd, RS485Serial, 0);
uint16_t ModbusSlaveRegisters[8];

void setup(){
    pinMode(CONTROLLINO_R0, OUTPUT);    // Set the pin CONTROLLINO_R0 as output.
    pinMode(CONTROLLINO_R1, OUTPUT);    // Set the pin CONTROLLINO_R1 as output.
    pinMode(CONTROLLINO_R2, OUTPUT);    // Set the pin CONTROLLINO_R2 as output.
    pinMode(CONTROLLINO_R3, OUTPUT);    // Set the pin CONTROLLINO_R3 as output.
    pinMode(CONTROLLINO_D0, INPUT);     // Set the pin CONTROLLINO_D0 as input - Read digital value at the pin D0.
    pinMode(CONTROLLINO_A0, INPUT);     // Set the pin CONTROLLINO_A0 as input - Read analog value at the pin A0

    ControllinoModbusSlave.begin( 38400UL );  // Start the communication over the ModbusRTU protocol. Baund rate is set at 19200.
}

// The loop function runs over and over again forever
void loop()
{
    // This instance of the class Modbus checks if there are any incoming data.
    // If any frame was received. This instance checks if a received frame is Ok.
    // If the received frame is Ok the instance poll writes or reads corresponding values to the internal registers (ModbusSlaveRegisters).
    // Main parameters of the instance poll are address of the internal registers and number of internal registers.
    ControllinoModbusSlave.poll(ModbusSlaveRegisters, 8);

    // While are not received or sent any data, the Modbus slave device periodically reads the values of analog and digital outputs.
    // Also it updates the other values of registers.
    ModbusSlaveRegisters[0] = analogRead(CONTROLLINO_A0);           // Read the analog input CONTROLLINO_A0.
    ModbusSlaveRegisters[1] = digitalRead(CONTROLLINO_D0);          // Read the digital input CONTROLLINO_A0.
    ModbusSlaveRegisters[2] = ControllinoModbusSlave.getInCnt();    // Read the number of incoming messages.
    ModbusSlaveRegisters[3] = ControllinoModbusSlave.getOutCnt();   // Read the number of outcoming messages

    digitalWrite(CONTROLLINO_R0, ModbusSlaveRegisters[4]); // Update the relay output CONTROLLINO_R0 - ON/OFF
    digitalWrite(CONTROLLINO_R1, ModbusSlaveRegisters[5]); // Update the relay output CONTROLLINO_R1 - ON/OFF
    digitalWrite(CONTROLLINO_R2, ModbusSlaveRegisters[6]); // Update the relay output CONTROLLINO_R2 - ON/OFF
    digitalWrite(CONTROLLINO_R3, ModbusSlaveRegisters[7]); // Update the relay output CONTROLLINO_R3 - ON/OFF
}

/* End of the example. Visit us at https://controllino.biz/ or contact us at info@controllino.biz if you have any questions or troubles. */

/* 2017-03-31: The sketch was successfully tested with Arduino 1.8.1, Controllino Library 1.1.0 and CONTROLLINO MAXI and MEGA. */

#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "ModbusRtu.h"    /* Usage of ModBusRtu library allows you to implement the Modbus RTU protocol in your sketch. */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define SlaveModbusAdd  1
#define RS485Serial     3

int dia , semana, mes, ano, hora, minuto, segundo;
bool bandera;
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

Modbus ControllinoModbusSlave(SlaveModbusAdd, RS485Serial, 0);

// This uint16 array specified internal registers in the Modbus slave device.
// Each Modbus device has particular internal registers that are available for the Modbus master.
// In this example sketch internal registers are defined as follows:
// (ModbusSlaveRegisters 0 - 3 read only and ModbusSlaveRegisters 4 - 7 write only from the Master perspective):
// ModbusSlaveRegisters[0] - Read the number of incoming messages - Communication diagnostic.
// ModbusSlaveRegisters[1] - Read the number of number of outcoming messages - Communication diagnostic.
// ModbusSlaveRegisters[2] - Sets the Day 
// ModbusSlaveRegisters[3] - Sets the WeekDay
// ModbusSlaveRegisters[4] - Sets the Month
// ModbusSlaveRegisters[5] - Sets the Year
// ModbusSlaveRegisters[6] - Sets the Hour
// ModbusSlaveRegisters[7] - Sets the Minute
// ModbusSlaveRegisters[8] - Sets the Second
uint16_t ModbusSlaveRegisters[9];

// The setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
// In the setup function is carried out port setting and initialization of communication of the Modbus slave protocol.
void setup(){
  ControllinoModbusSlave.begin( 19200 );  // Start the communication over the ModbusRTU protocol. Baund rate is set at 19200.
  Serial.begin(9600);
  Controllino_RTC_init();
  /* set time and date by separate values values to the RTC chip */
  /* Day, WeekDay, Month, Year, Hour, Minute, Second); */
  Controllino_SetTimeDate(12,4,1,17,15,41,23);

  lcd.init(); // initialize the lcd                      
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hola!");
  lcd.setCursor(2,1);
  lcd.print("CONTROLLINO"); 

}

// The loop function runs over and over again forever
void loop()
{
  int n;
    // This instance of the class Modbus checks if there are any incoming data.
    // If any frame was received. This instance checks if a received frame is Ok.
    // If the received frame is Ok the instance poll writes or reads corresponding values to the internal registers (ModbusSlaveRegisters).
    // Main parameters of the instance poll are address of the internal registers and number of internal registers.
    ControllinoModbusSlave.poll(ModbusSlaveRegisters, 9);

    // While are not received or sent any data, the Modbus slave device periodically reads the values of analog and digital outputs.
    // Also it updates the other values of registers.
    ModbusSlaveRegisters[0] = ControllinoModbusSlave.getInCnt();    // Read the number of incoming messages.
    ModbusSlaveRegisters[1] = ControllinoModbusSlave.getOutCnt();   // Read the number of outcoming messages
    Serial.println(ModbusSlaveRegisters[0]);
    Serial.println(ModbusSlaveRegisters[1]);

    dia = ModbusSlaveRegisters[2];
    semana = ModbusSlaveRegisters[3];
    mes = ModbusSlaveRegisters[4];
    ano = ModbusSlaveRegisters[5];
    hora = ModbusSlaveRegisters[6];
    minuto = ModbusSlaveRegisters[7];
    segundo = ModbusSlaveRegisters[8];
    
    Controllino_SetTimeDate(dia, semana, mes, ano, hora, minuto, segundo);
    Serial.print("Day: ");     n = Controllino_GetDay();     Serial.println(n);
    Serial.print("WeekDay: "); n = Controllino_GetWeekDay(); Serial.println(n);
    Serial.print("Month: ");   n = Controllino_GetMonth();   Serial.println(n);
    Serial.print("Year: ");    n = Controllino_GetYear();    Serial.println(n);
    Serial.print("Hour: ");    n = Controllino_GetHour();    Serial.println(n);
    Serial.print("Minute: ");  n = Controllino_GetMinute();  Serial.println(n);
    Serial.print("Second: ");  n = Controllino_GetSecond();  Serial.println(n);
    Serial.println("----------------------------------------------");
    date();
    delay(1000);
}


void date(){
  int n;
  lcd.setCursor(0,0);
  lcd.print("Date: ");
  n = Controllino_GetDay();    lcd.print(n); lcd.print("/");
  n = Controllino_GetMonth();  lcd.print(n); lcd.print("/");
  n = Controllino_GetYear();   lcd.print(n); lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print("Time: ");
  n = Controllino_GetHour();   lcd.print(n); lcd.print(":"); 
  n = Controllino_GetMinute(); lcd.print(n); lcd.print(":");
  n = Controllino_GetSecond(); lcd.print(n); lcd.print(" ");
}

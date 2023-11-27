#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "ModbusRtu.h"    /* Usage of ModBusRtu library allows you to implement the Modbus RTU protocol in your sketch. */
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define MasterModbusAdd  0
#define SlaveModbusAdd  1
#define RS485Serial     3

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

Modbus ControllinoModbusMaster(MasterModbusAdd, RS485Serial, 0);

// This uint16 array specified internal registers in the Modbus slave device.
// Each Modbus device has particular internal registers that are available for the Modbus master.
// In this example sketch internal registers are defined as follows:
// (ModbusSlaveRegisters 0 - 1 read only and ModbusSlaveRegisters 2 - 8 write only from the Master perspective):
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

// This is an structe which contains a query to an slave device
modbus_t ModbusQuery[2];

uint8_t myState; // machine state
uint8_t currentQuery; // pointer to message query

unsigned long WaitingTime;

void setup() {
   // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("-----------------------------------------");
  Serial.println("CONTROLLINO Modbus RTU Master Sync  & LCD");
  Serial.println("-----------------------------------------");
  Serial.println("");

  // ModbusQuery 0: read registers
  ModbusQuery[0].u8id = SlaveModbusAdd; // slave address
  ModbusQuery[0].u8fct = 3; // function code (this one is registers read)
  ModbusQuery[0].u16RegAdd = 0; // start address in slave
  ModbusQuery[0].u16CoilsNo = 2; // number of elements (coils or registers) to read
  ModbusQuery[0].au16reg = ModbusSlaveRegisters; // pointer to a memory array in the CONTROLLINO
  
  // ModbusQuery 1: write a single register
  ModbusQuery[1].u8id = SlaveModbusAdd; // slave address
  ModbusQuery[1].u8fct = 16; // function code (this one is write a single register)
  ModbusQuery[1].u16RegAdd = 2; // start address in slave
  ModbusQuery[1].u16CoilsNo = 7; // number of elements (coils or registers) to write
  ModbusQuery[1].au16reg = ModbusSlaveRegisters+2; // pointer to a memory array in the CONTROLLINO
  ModbusSlaveRegisters[4] = 1; // initial value for the RTC SLAVE
  
  ControllinoModbusMaster.begin( 19200 ); // baud-rate at 19200
  ControllinoModbusMaster.setTimeOut( 5000 ); // if there is no answer in 5000 ms, roll over
  
  WaitingTime = millis() + 1000;
  myState = 0;
  currentQuery = 0; 

  Controllino_RTC_init();
  Controllino_SetTimeDateStrings(__DATE__, __TIME__);
  
  lcd.init(); // initialize the lcd                      
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hola!");
  lcd.setCursor(2,1);
  lcd.print("CONTROLLINO");                     

}

void loop() {
  int n;

  if(Serial.available()){
    delay(1000);
    char c = Serial.read();
    switch( c ){
      case '0':
        Serial.println("---------- Sending query 0 ----------");
        ControllinoModbusMaster.query( ModbusQuery[0] );
          break;
      case '1':
          ModbusSlaveRegisters[2] = Controllino_GetDay(); 
          ModbusSlaveRegisters[3] = Controllino_GetWeekDay(); 
          ModbusSlaveRegisters[4] = Controllino_GetMonth(); 
          ModbusSlaveRegisters[5] = Controllino_GetYear(); 
          ModbusSlaveRegisters[6] = Controllino_GetHour(); 
          ModbusSlaveRegisters[7] = Controllino_GetMinute(); 
          ModbusSlaveRegisters[8] = Controllino_GetSecond(); 
          Serial.println("---------- Sending query 1 ----------");
          Serial.print("Day: ");     n = Controllino_GetDay();     Serial.println(n);
          Serial.print("WeekDay: "); n = Controllino_GetWeekDay(); Serial.println(n);
          Serial.print("Month: ");   n = Controllino_GetMonth();   Serial.println(n);
          Serial.print("Year: ");    n = Controllino_GetYear();    Serial.println(n);
          Serial.print("Hour: ");    n = Controllino_GetHour();    Serial.println(n);
          Serial.print("Minute: ");  n = Controllino_GetMinute();  Serial.println(n);
          Serial.print("Second: ");  n = Controllino_GetSecond();  Serial.println(n);
          ControllinoModbusMaster.query( ModbusQuery[1] ); // send query (only once)
          delay(500);
    }   
  }
  date();
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
  //delay(1000);
}

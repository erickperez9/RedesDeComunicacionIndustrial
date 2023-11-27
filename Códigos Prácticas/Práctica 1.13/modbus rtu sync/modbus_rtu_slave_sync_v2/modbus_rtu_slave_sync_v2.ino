#include <Controllino.h>
#include "ModbusRtu.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SlaveModbusAdd 1
#define RS485Serial 3

int dia, semana, mes, ano, hora, minuto, segundo;
bool dataReceived = false;  // A flag to track if data has been received from the master
bool clockStarted = false;  // A flag to track if the slave clock has started
int lastDia, lastSemana, lastMes, lastAno, lastHora, lastMinuto, lastSegundo;  // Variables to store the previous RTC values
LiquidCrystal_I2C lcd(0x27, 20, 4);

Modbus ControllinoModbusSlave(SlaveModbusAdd, RS485Serial, 0);

uint16_t ModbusSlaveRegisters[9];

void setup() {
  ControllinoModbusSlave.begin(19200);
  Serial.begin(9600);
  Controllino_RTC_init();
  Controllino_SetTimeDate(12, 4, 1, 17, 15, 41, 23);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Hola!");
  lcd.setCursor(2, 1);
  lcd.print("CONTROLLINO");
}

void loop() {
  int n;

  ControllinoModbusSlave.poll(ModbusSlaveRegisters, 9);

  ModbusSlaveRegisters[0] = ControllinoModbusSlave.getInCnt();
  ModbusSlaveRegisters[1] = ControllinoModbusSlave.getOutCnt();

  dia = ModbusSlaveRegisters[2];
  semana = ModbusSlaveRegisters[3];
  mes = ModbusSlaveRegisters[4];
  ano = ModbusSlaveRegisters[5];
  hora = ModbusSlaveRegisters[6];
  minuto = ModbusSlaveRegisters[7];
  segundo = ModbusSlaveRegisters[8];

  // Check if data has been received from the master and set the flag
  if (dia != 0 || semana != 0 || mes != 0 || ano != 0 || hora != 0 || minuto != 0 || segundo != 0) {
    dataReceived = true;
  }

  // If data has been received, update the RTC with the received values
  if (dataReceived) {
    if (!clockStarted || hasRTCChanged()) {
      // If the clock hasn't started or RTC values have changed, set the RTC
      Controllino_SetTimeDate(dia, semana, mes, ano, hora, minuto, segundo);
      lastDia = dia;
      lastSemana = semana;
      lastMes = mes;
      lastAno = ano;
      lastHora = hora;
      lastMinuto = minuto;
      lastSegundo = segundo;
      clockStarted = true;
    }
  }

  // Get current time from the RTC and continue running the clock
  //Controllino_GetTimeDate(dia, semana, mes, ano, hora, minuto, segundo);
  date();
  //printRCTvalues();
  //delay(1000);
}

bool hasRTCChanged() {
  // Check if RTC values have changed
  return (dia != lastDia || semana != lastSemana || mes != lastMes || ano != lastAno || hora != lastHora || minuto != lastMinuto || segundo != lastSegundo);
}

void date() {
  int n;
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  n = Controllino_GetDay();   lcd.print(n);  lcd.print("/");
  n = Controllino_GetMonth(); lcd.print(n);  lcd.print("/");
  n = Controllino_GetYear();  lcd.print(n);  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  n = Controllino_GetHour();   lcd.print(n);  lcd.print(":");
  n = Controllino_GetMinute(); lcd.print(n);  lcd.print(":");
  n = Controllino_GetSecond(); lcd.print(n);  lcd.print(" ");
}


void printRCTvalues(){
  // Print RTC values and update the LCD
  Serial.print("Day: ");  n = Controllino_GetDay();  Serial.println(n);
  Serial.print("WeekDay: ");  n = Controllino_GetWeekDay();  Serial.println(n);  
  Serial.print("Month: ");  n = Controllino_GetMonth();  Serial.println(n);
  Serial.print("Year: ");  n = Controllino_GetYear();  Serial.println(n);
  Serial.print("Hour: ");  n = Controllino_GetHour();  Serial.println(n);
  Serial.print("Minute: ");  n = Controllino_GetMinute();  Serial.println(n);
  Serial.print("Second: ");  n = Controllino_GetSecond();  Serial.println(n);
  Serial.println("----------------------------------------------");  
}

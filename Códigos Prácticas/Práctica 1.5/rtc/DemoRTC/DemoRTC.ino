#include <SPI.h>
#include <Controllino.h>  

void setup() {
  Serial.begin(9600);
  Controllino_RTC_init();

  /* set time and date by separate values values to the RTC chip */
  /* Day, WeekDay, Month, Year, Hour, Minute, Second); 
  Controllino_SetTimeDate(12,4,1,17,15,41,23); */
  
  /* following example uses predefined C macros __DATE__ and
  __TIME__ which represent compilation time */
  Controllino_SetTimeDateStrings(__DATE__, __TIME__); 
}

// the loop function runs over and over again forever
void loop() {
  Serial.print("Dia: "); Serial.println(int(Controllino_GetDay()));
  Serial.print("Semana: "); Serial.println(int(Controllino_GetWeekDay()));
  Serial.print("Mes: "); Serial.println(int(Controllino_GetMonth()));
  Serial.print("Año: "); Serial.println(int(Controllino_GetYear()));
  Serial.print("Hora: "); Serial.println(int(Controllino_GetHour()));
  Serial.print("Minuto: "); Serial.println(int(Controllino_GetMinute()));
  Serial.print("Segundo: "); Serial.println(int(Controllino_GetSecond()));
  Serial.println("**************************");
  delay(5000);      
}

#include <SPI.h>
#include <Ethernet.h>
#include <Timezone.h>
#include <Controllino.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 18, 101);
IPAddress myDns(8, 8, 8, 8);
EthernetClient client;

// NTP server details for Ecuador
char ntpServer[] = "ec.pool.ntp.org";
unsigned int localPort = 8888;  // Local port to listen for UDP packets

EthernetUDP Udp;  // A UDP instance to let us send and receive packets over UDP
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];  // Buffer to hold incoming and outgoing packets

TimeChangeRule myDST = {"DST", Last, Sun, Mar, 2, -300}; // UTC - 5 hours
TimeChangeRule mySTD = {"EST", First, Sun, Nov, 2, -300}; // UTC - 6 hours
Timezone myTZ(myDST, mySTD);

// Variables para almacenar la fecha y hora
int yearVar, monthVar, dayVar, hourVar, minuteVar, secondVar;

void setup() {
  Serial.begin(9600);
  Controllino_RTC_init();
  Controllino_SetTimeDate(12, 4, 1, 17, 15, 41, 23);
  // Inicializar Ethernet
  Serial.println("Inicializando Ethernet con DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Error al configurar Ethernet con DHCP");
    while (true) {
      delay(1000);
    }
  }

  // Inicializar UDP
  Udp.begin(localPort);

  Serial.println("Esperando la hora...");
  setSyncProvider(getNtpTime);
}

void loop() {
  if (timeStatus() != timeNotSet) {
    time_t utc = now();
    time_t local = myTZ.toLocal(utc);

    // Almacenar los valores de fecha y hora en variables
    yearVar = year(local);
    monthVar = month(local);
    dayVar = day(local);
    hourVar = hour(local);
    minuteVar = minute(local);
    secondVar = second(local);

    // Imprimir los valores almacenados
    Serial.print("Fecha y Hora: ");
    Serial.print(yearVar);
    Serial.print("-");
    printDigits(monthVar);
    Serial.print("-");
    printDigits(dayVar);
    Serial.print(" ");
    printDigits(hourVar);
    Serial.print(":");
    printDigits(minuteVar);
    Serial.print(":");
    printDigits(secondVar);

    // Calcular y mostrar la semana del mes
    int weekOfMonth = calculateWeekOfMonth(dayVar);
    Serial.print(" Semana del Mes: ");
    Serial.println(weekOfMonth);
    
    printRCTvalues();
    Controllino_SetTimeDate(dayVar, weekOfMonth, monthVar, yearVar% 100, hourVar, minuteVar, secondVar);
  }

  delay(1000);
}

void printDigits(int digits) {
  // Función de utilidad para mostrar el reloj digital: imprime dos puntos precedentes y cero principal
  Serial.print(':');
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

unsigned long getNtpTime() {
  Serial.println("Transmitir solicitud NTP");
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;   // LI, Versión, Modo
  packetBuffer[1] = 0;     // Estrato, o tipo de reloj
  packetBuffer[2] = 6;     // Intervalo de sondeo
  packetBuffer[3] = 0xEC;  // Precisión del reloj de pares
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  Udp.beginPacket(ntpServer, 123); // Las solicitudes NTP son para el puerto 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  delay(1000);

  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long secsSince1900;

    // Convertir cuatro bytes a partir de la ubicación 40 en un entero largo
    secsSince1900 = (unsigned long)packetBuffer[40] << 24;
    secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
    secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
    secsSince1900 |= (unsigned long)packetBuffer[43];

    return secsSince1900 - 2208988800UL;  // Devolver segundos desde 1900, ajustados para la época de Arduino
  }

  return 0; // Devolver 0 si no se puede obtener la hora
}

int calculateWeekOfMonth(int day) {
  // Calcular la semana del mes dividiendo el día por 7 y redondeando hacia arriba
  return (day + 6) / 7;
}


void printRCTvalues(){
  int n;
  Serial.print("Day: ");  n = Controllino_GetDay();  Serial.println(n);
  Serial.print("WeekDay: ");  n = Controllino_GetWeekDay();  Serial.println(n);  
  Serial.print("Month: ");  n = Controllino_GetMonth();  Serial.println(n);
  Serial.print("Year: ");  n = Controllino_GetYear();  Serial.println(n);
  Serial.print("Hour: ");  n = Controllino_GetHour();  Serial.println(n);
  Serial.print("Minute: ");  n = Controllino_GetMinute();  Serial.println(n);
  Serial.print("Second: ");  n = Controllino_GetSecond();  Serial.println(n);
  Serial.println("----------------------------------------------");  
}

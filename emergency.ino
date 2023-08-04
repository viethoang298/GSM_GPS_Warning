#include <TinyGPS++.h>
#include <SoftwareSerial.h>
TinyGPSPlus gps;
SoftwareSerial gpsSerial(2, 3);
SoftwareSerial gsmSerial(4, 5);

#define PushButton 7
#define Switch 8

bool mode;
bool buttonstate = 1;

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  gpsSerial.begin(9600);
  
  pinMode(PushButton, INPUT_PULLUP);
  pinMode(Switch, INPUT_PULLUP);

  //gsmSerial.println("AT+CMGF=1");
    //      gsmSerial.println("AT+CMGS=\"+84963435225\"");
      //    gsmSerial.print("Send help");
        //  gsmSerial.write(26);
          //updateSerial();
}

void loop() {
  mode = digitalRead(Switch);

  if (digitalRead(PushButton) == 0) {
    buttonstate = 0;
  }

  if ((mode == 1) && (buttonstate == 0)) { //Call Mode
    Serial.println("Calling...");
    gsmSerial.println("ATD+ +84963435225;");
    delay(20000);
    gsmSerial.println("ATH");
    buttonstate = 1;
  }

  if ((mode == 0) && (buttonstate == 0)) { //GPS SMS Mode
    Serial.println("Texting...");
    gsmSerial.println("AT+CMGF=1");
    updateSerial();
          gsmSerial.println("AT+CMGS=\"+84963435225\"");
          updateSerial();
          gsmSerial.print("Send help");
          updateSerial();
          gsmSerial.write(26);
          updateSerial();
    buttonstate = 1;
    while (gpsSerial.available() > 0)
      if (gps.encode(gpsSerial.read())) {
        if (gps.location.isValid()) {
          gsmSerial.println("AT+CMGF=1");
          updateSerial();
          gsmSerial.println("AT+CMGS=\"+84963435225\"");
          updateSerial();
          gsmSerial.print(gps.location.lat(), 6);
          updateSerial();
          gsmSerial.print(" ");
          updateSerial();
          gsmSerial.print(gps.location.lng(), 6);
          updateSerial();
          gsmSerial.write(26);
          updateSerial();
          buttonstate = 1;
        }
      }
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    gsmSerial.write(Serial.read());//Forward what Serial received to Software Serial Port
    gpsSerial.write(Serial.read());
  }
  while(gsmSerial.available()) 
  {
    Serial.write(gsmSerial.read());//Forward what Software Serial received to Serial Port
  }
  while(gpsSerial.available()) 
  {
    Serial.write(gpsSerial.read());//Forward what Software Serial received to Serial Port
  }
}

void displayInfo()
{
  if (gps.location.isValid())
  {
    Serial.print("Latitude: ");      
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
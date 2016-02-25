#include <QueueList.h>
#include <LiquidCrystal.h>


QueueList <byte> PackageQueue;
char inByte;
String packageId, timeStr, latitude, longitude, quality, 
       nSat, hdop, altitude, wgs84, latitudeOrienta, longitudeOrienta;
int iterator;
LiquidCrystal lcd(53, 51, 52, 50, 48, 46);

void setup() {
  Serial.begin(115200);               //USB Serial                                  
  Serial1.begin (115200);             //Serial1 TX RX
  Serial.println("USB Ready!");
  lcd.begin(16,2); //LCD
  lcd.setCursor(0,0);
  lcd.print("Ready");
}

void loop() {
  while (Serial1.available () > 0) { 
     //Serial.println(Serial1.read());
     inByte = Serial1.read();
     if (inByte == '$') {
        packageId = Serial1.readStringUntil(',');
        if (packageId.compareTo("GPGGA") == 0) {
          while (iterator == 10) {
            //Serial.println(Serial1.readStringUntil('$'));
            Serial.print("Time: ");
            timeStr = Serial1.readStringUntil(',');
            Serial.println(timeStr);
            
            Serial.print("Latitude: ");
            latitude = Serial1.readStringUntil(','); // + N
            Serial.println(latitude);
            latitudeOrienta = Serial1.readStringUntil(',');
            
            Serial.print("Longitude: ");
            longitude = Serial1.readStringUntil(','); // + W
            Serial.println(longitude);
            longitudeOrienta = Serial1.readStringUntil(',');
            
            Serial.print("Quality: ");
            quality = Serial1.readStringUntil(','); 
            Serial.println(quality);

            Serial.print("N Satelittes: ");
            nSat = Serial1.readStringUntil(','); 
            Serial.println(nSat);

            Serial.print("HDOP: ");
            hdop = Serial1.readStringUntil(','); 
            Serial.println(hdop);

            Serial.print("Altitude: ");
            altitude = Serial1.readStringUntil(','); // + M
            Serial.println(altitude);
            Serial1.readStringUntil(',');

            Serial.println("-------------------------------");
            lcd.setCursor(0,0);
            lcd.print(longitude.substring(0,7));
            lcd.setCursor(7,0);
            lcd.print(longitudeOrienta);
            lcd.setCursor(8,0);
            lcd.print(latitude.substring(0,7));
            lcd.print(latitudeOrienta);
            
            lcd.setCursor(0,1);
            lcd.print(altitude.substring(0,6));
            lcd.setCursor(5,1);
            lcd.print("M");
            lcd.setCursor(9,1);
            lcd.print("Q");
            lcd.setCursor(10,1);
            lcd.print(quality);
            lcd.setCursor(13,1);
            lcd.print("S");
            lcd.setCursor(14,1);
            lcd.print(nSat);
            
            iterator = 0;
          }
          iterator++;
        }
     }           
  }
}




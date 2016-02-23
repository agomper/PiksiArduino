#include <QueueList.h>
QueueList <byte> PackageQueue;

void setup() {
  Serial.begin(115200);                                
  Serial1.begin (115200);
  Serial.println("USB Ready!");
}

void loop() {
  while (Serial1.available () > 0) {
    processByte (Serial1.read ());           
  }
}

void processByte (const byte inByte) {
   static int PosCounter = 0, PackageLength = 10;
   static bool NewPackage = false;
   
   if (inByte == 0x55) {
      //Serial.print("New Package!");
      PosCounter = 0;
      NewPackage = true;
      PackageQueue.push(inByte);
      //PosCounter++;
   }
   else if (NewPackage == true) {
      PackageQueue.push(inByte);
      PosCounter++;
      if (PosCounter == 5) {
        PackageLength = inByte + 8;
        //Serial.print(PackageLength);
      }
   }

   if (PosCounter == PackageLength - 1) {
      while (PackageQueue.isEmpty() == false) {
        Serial.print(PackageQueue.peek(), HEX);
        Serial.print(" ");
        PackageQueue.pop();
      }
      Serial.println();
      PosCounter = 0;
      NewPackage = false;
   }
}


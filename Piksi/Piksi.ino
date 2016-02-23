#include <QueueList.h>
QueueList <byte> PackageQueue;

void setup() {
  Serial.begin(115200);                                
  Serial1.begin (115200);
  Serial.println("USB Ready!");
}

void loop() {
  while (Serial1.available () > 0) {
    process_byte (Serial1.read ());           
  }
}

void process_byte (const byte inByte) {
   static int PackageLength = 10;
   static bool NewPackage, MessageCode1, MessageCode2 = false;
   
   if (inByte == 0x55) {
      //Serial.print("New Package!");
      NewPackage = true;
      PackageQueue.push(inByte);
   }
   else if (NewPackage == true) {
      if (PackageQueue.count() == 1 && inByte == 2) {
        //Serial.print("MessageCode1 True");
        //Serial.println();
        MessageCode1 = true;
        PackageQueue.push(inByte);
      }
      else if (PackageQueue.count() == 2 && inByte == 2 && MessageCode1 == true) {
        //Serial.print("MessageCode2 True");
        //Serial.println();
        MessageCode2 = true;
        PackageQueue.push(inByte);
      }
      else if (MessageCode2 == true) {
        if (PackageQueue.count() == 5)
          PackageLength = inByte + 8;
        PackageQueue.push(inByte);
      }
      else if (MessageCode2 == false){
        clean_queue();
        NewPackage = false;
      }
   }

   if (PackageQueue.count() == PackageLength - 1) {
      show_queue();
      NewPackage = false;
      MessageCode1 = MessageCode2 = false;
   }
}

void show_queue() {
    while (PackageQueue.isEmpty() == false) {
        Serial.print(PackageQueue.peek(), HEX);
        Serial.print(" ");
        PackageQueue.pop();
    }
    Serial.println();
}


void clean_queue() {
    while (PackageQueue.isEmpty() == false) {
        PackageQueue.pop();
    }
}


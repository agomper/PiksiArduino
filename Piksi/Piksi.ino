#include <QueueList.h>
//#include <IEEE754tools.h>
QueueList <byte> PackageQueue;

void setup() {
  Serial.begin(115200);               //USB Serial                                  
  Serial1.begin (115200);             //Serial1 TX RX
  Serial.println("USB Ready!");
}

void loop() {
  while (Serial1.available () > 0) {  //While there is information coming to Serial1 RX, I process it.
    process_byte (Serial1.read ());           
  }
}

void process_byte (const byte inByte) {
   static int PackageLength = 42;
   static bool NewPackage, MessageCode1, MessageCode2 = false;
   
   if (inByte == 0x55) {                //SBP Preamble
      //Serial.print("New Package!");
      NewPackage = true;
      PackageQueue.push(inByte);        //Insert the byte in the queue
   }
   else if (NewPackage == true) {
      if (PackageQueue.count() == 1 && inByte == 0x03) {     //Code HEX Message 1 - Message type are in positions 1 and 2
        //Serial.print("MessageCode1 True");
        //Serial.println();
        MessageCode1 = true;
        PackageQueue.push(inByte);      //Insert the byte in the queue
      }
      else if (PackageQueue.count() == 2 && inByte == 0x02 && MessageCode1 == true) {  //Code HEX Message 2
        //Serial.print("MessageCode2 True");
        //Serial.println();
        MessageCode2 = true;
        PackageQueue.push(inByte);      //Insert the byte in the queue
      }
      else if (MessageCode2 == true) {  //Message Type Match
        if (PackageQueue.count() == 5)  //Length byte in position 5
          PackageLength = inByte + 8;   //Length = Payload + 8 = Fixed bytes (Preamble, Type, Sender, Length and CRC)
        PackageQueue.push(inByte);
      }
      else if (MessageCode2 == false){ //No match message type. Erase.
        clean_queue();
        NewPackage = false;
        MessageCode1 = false;
      }
   }

   if (PackageQueue.count() == PackageLength) { //Got all the package. Payload + 8
      //show_queue();
      process_message(PackageQueue.count()); //Process Message
      NewPackage = false;
      MessageCode1 = MessageCode2 = false; //Reset values
   }
}

void show_queue() { //It shows the whole message from Preamble to CRC
    while (PackageQueue.isEmpty() == false) {
        Serial.print(PackageQueue.peek(), HEX);
        Serial.print(" ");
        PackageQueue.pop();
    }
    Serial.println();
}


void clean_queue() { //Erase the queue
    while (PackageQueue.isEmpty() == false) {
        PackageQueue.pop();
    }
}

void process_message(int MesLength) { //Cut the message in pieces
  byte buffer_4bytes[4];
  
  for (int i=0; i<10; i++) {
    PackageQueue.pop();
  }
  
  for (int i=3; i!=0; i--) {
    buffer_4bytes[i] = PackageQueue.pop();
  }
  bytes4_to_int(buffer_4bytes);
  

  clean_queue();
}

int bytes4_to_int (byte aux_buffer[4]) { //Error. Mega and 64 bits double are not friends.
  /*uint32_t res = 0;
  for (int i = 0 ; i != 4 ; i++) {
      res <<= 8;
      res |= aux_buffer[i];
  }
  Serial.println(res);*/
  
  long l;
  memcpy(&l, aux_buffer, sizeof(long));

  Serial.println(sizeof(aux_buffer));

}



// UTFT_ViewFont 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the included fonts.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t Dingbats1_XL[];


// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ITDB32S,38,39,40,41);
UTouch        myTouch(6,5,4,3,2);
UTFT_Buttons  myButtons(&myGLCD, &myTouch);

String buffer;
char preamble;
int iterator=0;
  
void setup() {
  myGLCD.InitLCD();
  myGLCD.clrScr();

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  myButtons.setTextFont(BigFont);
  myButtons.setSymbolFont(Dingbats1_XL);

  Serial.begin(115200);
  Serial.println("USB Ready!");
}

void loop() {
  int buttonSave, buttonDistance, pressedButton = 999;
  bool saveToFile = false, calculateDistance = false;
  char dataMode = 'N';
  buttonSave = myButtons.addButton( 0, 140, 150,  30, "Save");
  buttonDistance = myButtons.addButton( 160, 140, 150,  30, "Move");
  myButtons.drawButtons();
  
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 0);

  //Cada salto son 16 filas.
  myGLCD.setFont(BigFont);
  myGLCD.print("North:", LEFT, 0);
  myGLCD.print("East:", LEFT, 32);
  myGLCD.print("D.Base:", RIGHT, 32);
  myGLCD.print("Down:", LEFT, 64);
  myGLCD.print("N. Sats:", LEFT, 96);
  myGLCD.print("Mode:", LEFT, 112);

  while(1) {
    if (myTouch.dataAvailable() == true) {
       pressedButton = myButtons.checkButtons();
    }

    if (pressedButton == buttonSave) {
       pressedButton = 999;
       if (saveToFile == false) {
          saveToFile = true;
          myGLCD.print("Saving to file", LEFT, 175);
          myButtons.disableButton(buttonDistance);
          dataMode = 'S';
       }
       else {
          saveToFile = false;
          myGLCD.setColor(0, 0, 0);
          myGLCD.fillRect(0,175,319,191);
          myGLCD.setColor(0, 255, 0);
          myButtons.enableButton(buttonDistance);
          dataMode = 'N';
       }
    }
    if (pressedButton == buttonDistance) {
       pressedButton = 999;
       if (calculateDistance == false) {
          calculateDistance = true;
          myGLCD.print("Distance:", LEFT, 175);
          myButtons.disableButton(buttonSave);
          dataMode = 'D';
       }
       else {
          calculateDistance = false;
          myGLCD.setColor(0, 0, 0);
          myGLCD.fillRect(0,175,319,191);
          myGLCD.setColor(0, 255, 0);
          myButtons.enableButton(buttonSave);
          dataMode = 'N';
       }
    }
    
    
    while (Serial.available () > 0) {
       preamble = Serial.read();
       if (preamble == '$') {
           myGLCD.setColor(0, 0, 0);
           // X1 / Y1 / X2 / Y2
           myGLCD.fillRect(0,16,319,31);
           myGLCD.fillRect(0,48,319,63);
           myGLCD.fillRect(0,80,319,95);
           myGLCD.fillRect(160,112,319,128);
           myGLCD.fillRect(0,191,319,206);
           myGLCD.fillRect(0,207,319,223);
           myGLCD.setColor(0, 255, 0);

           buffer = Serial.readStringUntil(',');
           myGLCD.print(buffer, LEFT, 16);
           buffer = Serial.readStringUntil(',');
           myGLCD.print(buffer, LEFT, 48);
           buffer = Serial.readStringUntil(',');
           myGLCD.print(buffer, LEFT, 80);
           buffer = Serial.readStringUntil(',');
           myGLCD.print(buffer, 160, 96);
           buffer = Serial.readStringUntil(',');
           myGLCD.print(buffer, 160, 112);
           buffer = Serial.readStringUntil('&');
           myGLCD.print(buffer, RIGHT, 48);
           
           //if (calculateDistance == true) {
              //myGLCD.print(buffer, LEFT, 191);
              //myGLCD.print("Meters", LEFT, 207);
           //}
           Serial.write(dataMode);
           Serial.flush();
           
           iterator=0;
       }
       else if (preamble == '#') {
          myGLCD.setColor(0, 0, 0);
          myGLCD.fillRect(0,191,319,206);
          myGLCD.setColor(0, 255, 0);
          
          buffer = Serial.readStringUntil('&');
          myGLCD.print(buffer, LEFT, 191);
          myGLCD.print("meters", RIGHT, 191); //207 is the next line

          Serial.write(dataMode);
          Serial.flush();
       }
       
    }
  };
}


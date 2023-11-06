// Code for the transmitter (steering wheel) of the FPV Rally Racing car

// To compile - first setup arduino as follows:
// File | Preferences -> then add this url into additional board managers: https://dl.espressif.com/dl/package_esp32_index.json
// Tools | Board | Board Manager -> search for ESP32 and intstall
// Tools | Board | ESP32 Dev Module
// Requires external libraries:
// (from Tools/Manage Libraries): Adafruit MPU6050 (plus BUSIO), U8G2 library for graphics, MD_REncoder for encoder knob, ESP32Servo (for receiver)
// (To download manually): FastCRC

#include <Arduino.h>
#include "Radio.h"
#include "Menu.h"
#include "FPV.h"
//#include <Update.h> // Have removed these for now but this allows code to be updated over wifi
//#include "Updater.h"


    // change these values to change default speed and steering
    int defaultSpeed = 50;
    int defaultSteering = 90;




void startupScreen() {
  // Write all your code for the startup screen in here.
  // This will run before anything else .

  

  u8g2.begin();
  u8g2.setFont(u8g2_font_7x13B_mr);
  for(int i = 0; i <= u8g2.getWidth() + 2; i += 5)
  {
    u8g2.clearBuffer();
    u8g2.drawStr(i, u8g2.getHeight() / 2, "TF");
    u8g2.sendBuffer();
    delay(2);
    
  }
    delay(1000);
      
}


MenuItem *mainMenu;

void packetRx() // This gets called when a packet is received from the radio - it's blank because the transmitter is a transmitter, not a receiver! 
{
}

void menuChange(const char *itemName, int value) // This gets called when a menu item is selected or a parameter is changed
{
  delay(300); // have just hit select, so give time to release ...
  if((strcmp("Drive",itemName)==0) && isCalibrated())
  {
    int throttleMax = mainMenu->getItem("Throttle")->defaultVar;
    int steeringMax = mainMenu->getItem("Steering")->defaultVar;
    bindIfNeeded();



    int randomY = 10;
    int speedY = 5;
    int smallerRandomY = 10;
    int smallerSpeedY = 4;
    int smallerRandomY2 = 10;
    int smallerSpeedY2 = 3;
    int smallerRandomY3 = 10;
    int smallerSpeedY3 = 2;
    int smallerRandomY4 = 10;
    int smallerSpeedY4 = 1;

    
    while(digitalRead(ENC_SW)==HIGH) // While encoder knob not pressed ...
    {
      
      // Check if any red buttons pressed, and if so put the button number in the 'command' parameter to send to the receiver
      if(digitalRead(d1)==LOW){
        outgoingRadioPacket.command = 1;
      }
      else if(digitalRead(d2)==LOW){
        outgoingRadioPacket.command = 2;
      }
      else if(digitalRead(d3)==LOW){
        outgoingRadioPacket.command = 3;

      }
        
      else if(digitalRead(d4)==LOW)
        outgoingRadioPacket.command = 4;
      else if(digitalRead(d5)==LOW)
        outgoingRadioPacket.command = 5;
      else if(digitalRead(d6)==LOW)
        outgoingRadioPacket.command = 6;
      else
        outgoingRadioPacket.command = 0;
        
      outgoingRadioPacket.throttle = getThrottle(defaultSpeed);////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      outgoingRadioPacket.angle = getSteering(steeringMax);
      u8g2.clearBuffer();
      u8g2.setCursor(0,10);
      u8g2.setFont(u8g2_font_7x13B_mr);
      u8g2.print(outgoingRadioPacket.throttle);
      u8g2.setCursor(0,24);
      u8g2.print(outgoingRadioPacket.angle);
      u8g2.setCursor(0,44);
      u8g2.print(defaultSpeed);
      
  if ((digitalRead(swA)==LOW) && (digitalRead(swB)==LOW)){
    defaultSpeed += 5;
    if (defaultSpeed > 100){
      defaultSpeed = 5;
    }
  }
      u8g2.sendBuffer();
      sendPacket();
      delay(5 + random(0,11)); // delay for an average of 10ms +/- 5ms - this variation prevents regular collisions
    }    
  }
/*  
  // Code to update over wifi - comment out to reduce compile time if just programming over USB
  if(strcmp("Update Code",itemName)==0)
  { // Wifi code update facility - ssid and password for access point must be set in Updater.cpp
    u8g2.clearBuffer(); u8g2.drawStr(0, 16,"Wifi Cnct ..."); u8g2.sendBuffer();
    if(updaterSetup())
    { u8g2.clearBuffer(); u8g2.drawStr(0, 16,"Update Mode");
      u8g2.setFont(u8g2_font_7x13B_mr); u8g2.drawStr(0,40,myIPAddress.toString().c_str()); u8g2.sendBuffer();
      while(1) updaterLoop(); }
    else
    { u8g2.clearBuffer(); u8g2.drawStr(0, 16,"Wifi Timeout"); u8g2.sendBuffer(); }
  }*/
  if(strcmp("Test",itemName)==0)
    doTest();
  if(strcmp("Calibrate",itemName)==0)
    calibrateThrottle();
  delay(300); // if they've hit select, give time to release before continuing with menu system
}

void setup()
{
  startupScreen();
  FPVSetup(); // Sets up the hardware and runs a test mode if one of the bottom paddles is pressed
  
  // This sets up the menu system.  Items on the main menu either just execute code or allow parameters to be set
  // The ones below with numbers are parameters
  // For example, the line:  mainMenu->addItem("Config","Throttle",5,100,30,5); ...
  // .. sets up a parameter called 'Throttle' that can go between 5 and 100, with a default value of 30, and an increment of 5    
  mainMenu = new MenuItem("Main Menu");
  mainMenu->addItem("Main Menu","Drive");
  mainMenu->addItem("Main Menu","Config");


    
    mainMenu->addItem("Config","Throttle",5,100,defaultSpeed,5);    
    mainMenu->addItem("Config","Steering",5,90,defaultSteering,5);    
    mainMenu->addItem("Config","Calibrate");    
    mainMenu->addItem("Config","Update Code");    
    mainMenu->addItem("Config","Back");    
  mainMenu->addItem("Main Menu","Test");
  mainMenu->addItem("Main Menu","Back");
  mainMenu->activate(menuChange);
  
  setupRadio(1,packetRx); // Sets up the radio, and calls packetRx() when it receives a command
}

void loop()
{
  menuService(); // Must call this regularly so that the menu and rotary encoder works
}

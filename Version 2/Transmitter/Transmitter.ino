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

void startupScreen() {
  // Write all your code for the startup screen in here.
  // This will run before anything else .

  u8g2.begin();
  u8g2.setFont(u8g2_font_7x13B_mr);
  u8g2.clearBuffer();

  // modify name here
  int y=30;

  u8g2.clearBuffer();
  for(int x=0;x<=u8g2.getWidth();x+=3)
  {
    u8g2.drawStr(x,y,"Jeremy");
    y-=3;
    u8g2.sendBuffer();
    delay(2);
    u8g2.clearBuffer();
    if(y<=0)
    {
      for(int t=1;t<=u8g2.getHeight();t+=4)
      {
        u8g2.drawStr(x,t,"Jeremy");
        u8g2.sendBuffer();
        delay(2);
        u8g2.clearBuffer();
      }
      y=u8g2.getHeight();
    }
    
  }

  u8g2.clearBuffer();
  
  //u8g2.drawStr(30,sin(10),"Jeremy");
 // u8g2.sendBuffer();
 // delay(50);
 /// u8g2.clearBuffer();
 //  u8g2.drawStr(60,sin(40),"Jeremy");
  //u8g2.sendBuffer();
 // delay(50);
 // u8g2.clearBuffer();
  


  
  // animation 1: tv screen ball
  
  int time=50;
  
  int dotx=0;
  int doty=0;

  int speedx =5;
  int speedy =2;
  
  
  while (time>0){
    u8g2.clearBuffer();

    u8g2.drawDisc(dotx, doty, 3);
    u8g2.sendBuffer();

    dotx += speedx;
    doty += speedy;

    if (dotx < 0 || dotx >= u8g2.getWidth()){
      speedx *= -1;
    }

    
    if (doty < 0 || doty >= u8g2.getHeight()){
      speedy *= -1;
    }

    

    delay(5);

    time-=1;
  }



   
  
}

MenuItem *mainMenu;

void packetRx() // This gets called when a packet is received from the radio - it's blank because the transmitter is a transmitter, not a receiver! 
{
}

void menuChange(const char *itemName, int value) // This gets called when a menu item is selected or a parameter is changed
{
  delay(300); // have just hit select, so give time to release ...
  if((strcmp("Race!",itemName)==0) && isCalibrated())
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
        if (throttleMax<=95){
          throttleMax+=5;
          u8g2.drawStr(20,20,"⬆");
          delay(30);
          if (throttleMax>=70)
          {
            steeringMax=40;
          }
          if ((throttleMax<70)&&(throttleMax>50))
          {
            steeringMax=50;
          }
          if ((throttleMax<50)&&(throttleMax>30))
          {
            steeringMax=60;
          }
        }
      }
      else if(digitalRead(d3)==LOW){
        outgoingRadioPacket.command = 3;
      }
      else if(digitalRead(d4)==LOW){
        outgoingRadioPacket.command = 4;
      }
      else if(digitalRead(d5)==LOW){
        outgoingRadioPacket.command = 5;
        if (throttleMax>=35){
          throttleMax-=5;
          delay(30);
        }
      }
      else if(digitalRead(d6)==LOW){
        outgoingRadioPacket.command = 6;
      }
      else
        outgoingRadioPacket.command = 0;
        
      outgoingRadioPacket.throttle = getThrottle(throttleMax);
      outgoingRadioPacket.angle = getSteering(steeringMax);
      u8g2.clearBuffer();
      u8g2.setCursor(0,10);
      u8g2.setFont(u8g2_font_7x13B_mr);
      u8g2.print(outgoingRadioPacket.throttle);
      u8g2.setCursor(0,24);
      u8g2.print(outgoingRadioPacket.angle);


      // add race screen info here
      
//      u8g2.drawDisc(60, randomY, 10);
//      u8g2.drawDisc(80, randomY, 5);
//      u8g2.drawDisc(95, randomY, 3);
//      u8g2.drawDisc(105, randomY, 2);
//      u8g2.drawDisc(110, randomY, 1);      

      u8g2.drawDisc(60, randomY, 10);
      
      // update randomyY 

      randomY += speedY;

      if (randomY>50 || randomY<10){
          speedY *=-1;
      }


      u8g2.drawDisc(80, smallerRandomY, 5);

      // update this too :)

      smallerRandomY += smallerSpeedY;

      if (smallerRandomY>50 || smallerRandomY<10){
          smallerSpeedY *=-1;
      }


      u8g2.drawDisc(95, smallerRandomY2, 3);

      // update this too :)

      smallerRandomY2 += smallerSpeedY2;

      if (smallerRandomY2>50 || smallerRandomY2<10){
          smallerSpeedY2 *=-1;
      }

      u8g2.drawDisc(105, smallerRandomY3, 2);

      // update this too :)

      smallerRandomY3 += smallerSpeedY3;

      if (smallerRandomY3>50 || smallerRandomY3<10){
          smallerSpeedY3 *=-1;
      }

      u8g2.drawDisc(110, smallerRandomY4, 1);

      // update this too :)

      smallerRandomY4 += smallerSpeedY4;

      if (smallerRandomY4>50 || smallerRandomY4<10){
          smallerSpeedY4 *=-1;
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
  mainMenu->addItem("Main Menu","Race!");
  mainMenu->addItem("Main Menu","Config");

    // change these values to change default speed and steering
    int defaultSpeed = 30;
    int defaultSteering = 60;

    
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

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

  u8g2.clearBuffer();

  u8g2.drawStr(20, 24, "Geh ma");

  u8g2.sendBuffer();

  delay(1000);



  
  u8g2.clearBuffer();


  // animation 1: tv screen ball

  //int time=10;
  //int dotx=0;
  //int doty=0;

  //int speedx =5;
  //int speedy =2;
  
  
  //while (time>0){
    //u8g2.clearBuffer();

    //u8g2.drawDisc(dotx, doty, 3);
    //u8g2.sendBuffer();

    //dotx += speedx;
    //doty += speedy;

    //if (dotx < 0 || dotx >= u8g2.getWidth()){
      //speedx *= -1;
    //}

    
    //if (doty < 0 || doty >= u8g2.getHeight()){
      //speedy *= -1;
    //}

    

    //delay(50);

    //time-=1;
  //}


  // animation 2, pendulum

  //const int numBalls = 3;
  //const int ballRadius = 3;
  //const int ballGap = 15;
  //const int rodLength = 30;
  //const int centerX = 64;
  //const int centerY = 5;

  //float ballX[numBalls] = {centerX, centerX - ballGap, centerX};
  //float ballY[numBalls] = {centerY, centerY + rodLength, centerY + rodLength};

  //float ballVX[numBalls] = {0,0,-1};
  //float ballVY[numBalls] = {0,0,0};

  //const float g = 0.1;


  //time = 100;

  //while (time>0){

    // update

    //for (int i=1; i < numBalls; i++){

      //ballVY[i] += g;

      //ballX[i] += ballVX[i];
      //ballY[i] += ballVY[i];

      //if (ballY[i] >= u8g2.getHeight() - ballRadius){
        //ballVY[i] *= -0.9;
        //ballY[i] = u8g2.getHeight()-ballRadius;
      //}
    //}
    
    // update done

    // check for collisions
    //int dx = ballX[2] - ballX[1];
    //int dy = ballY[2] - ballY[1];
    //int distance = sqrt(dx*dx + dy*dy);

    //if (distance<2*ballRadius){

      //float tempVX = ballVX[1];
      //float tempVY = ballVY[1];
      //ballVX[1] = ballVX[2];
      //ballVY[1] = ballVY[2];
      //ballVX[2] = tempVX;
      //ballVY[2] = tempVY;
    //}



    //if ball speed approaches 0, reverse direction
    //if (abs(ballVX[1])<0.6 && abs (ballVX[1]!=0)){
      //ballVX[1] *= -1;
      //ballVX[1] += 0.4;
    //}
    
    //if (abs(ballVX[2])<0.6 && abs(ballVX[2]!=0)){
      //ballVX[2] *= -1;
      //ballVX[2] += 0.4;
    //}

    // if ball near edge, make it go backwards

    //if (ballX[1]<5){
      //ballVX[1] *= -1;
    //}
   //if (ballX[2]>100){
     // ballVX[2] *= -1;
    //}
    
    
    // drawPendulum

    //u8g2.clearBuffer();


    //for (int i = 0; i < numBalls; i++){
      //u8g2.drawDisc(ballX[i], ballY[i], ballRadius);
    //}

    //for (int i = 1; i < numBalls; i++){
      //u8g2.drawLine(ballX[i], ballY[i], centerX, centerY);
    //}

    //u8g2.sendBuffer();


    //time -= 1;
  //}
     

   
  
//}

MenuItem *mainMenu;

void packetRx() // This gets called when a packet is received from the radio - it's blank because the transmitter is a transmitter, not a receiver! 
{
}

void menuChange(const char *itemName, int value) // This gets called when a menu item is selected or a parameter is changed
{
  delay(300); // have just hit select, so give time to release ...
  if((strcmp("Race!",itemName)==0) && isCalibrated())}
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
      if(digitalRead(d1)==LOW)
        outgoingRadioPacket.command = 1;
      else if(digitalRead(d2)==LOW)
        outgoingRadioPacket.command = 2;
      else if(digitalRead(d3)==LOW)
        outgoingRadioPacket.command = 3;
      else if(digitalRead(d4)==LOW)
        outgoingRadioPacket.command = 4;
      else if(digitalRead(d5)==LOW)
        outgoingRadioPacket.command = 5;
      else if(digitalRead(d6)==LOW)
        outgoingRadioPacket.command = 6;
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

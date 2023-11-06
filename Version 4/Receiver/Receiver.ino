// This code gets programmed into the FPV Rally receiver board that controls the steering and motor on the RC car
// It receives signals from the transmitter (the motion-controlled steering wheel)
// It's pretty simple as it just waits for a transmitter to go into race mode and pairs with it ...
// ... and then just takes the incoming steering and throttle settings and controls the servo/ESC for the motor

#include <Wire.h>
#include "Radio.h"
#include <ESP32Servo.h>

Servo throttle;
Servo steering;
int speed=90;           // My current speed.  90 means not moving. >90 is forward and <90 is backwards
unsigned long lastrx=0; // Used to keep track of when we last received some info from the radio
uint16_t unitNum=0;     // 15-bit number telling me what unit number I am - transmitter creates a random number on first going into race mode when powered on and transmits it to the receiver 
int moveSequence=0;     // Contains number if red buttons 1-5 pressed (button 6 cancels the move and sets moveSequence to 0)

void packetRx() // Called when we receive a packet from the transmitter
{
  int i;
  /*Serial.print(incomingRadioPacket.unitNum);
  Serial.print(' ');
  Serial.print(incomingRadioPacket.throttle);
  Serial.print(' ');
  Serial.println(incomingRadioPacket.angle);*/
  if((unitNum==0) && (incomingRadioPacket.unitNum & 0x8000)) // If not bound and high bit set from transmitter (means it's within one second of first going into 'Race' mode ...
  {
    unitNum = incomingRadioPacket.unitNum & 0x7fff; // Set the unit number
    // Now for half a second go gently forward/backward to create a buzz from the motor to signal that we have bound
    for(i=1; i<250; i++)
    {
      throttle.write(100);
      delay(1);
      throttle.write(80);
      delay(1);
    }
    throttle.write(90);
  }
  else if (unitNum==incomingRadioPacket.unitNum) // once we have bound, only listen to incoming packets destined for us
  {
    //HOW TO CANCEL BUTTON
    if(incomingRadioPacket.command==6)   // button 6 pressed which should cancel move sequence if running
      moveSequence=0;

      //OTHERWISE SET THE MOVE SEQUENCE TO THE BUTTON YOU PRESSED
    else if(incomingRadioPacket.command){ // button 1-5 pressed, so set the moveSequence to this number
      moveSequence=incomingRadioPacket.command;//SET THE MOVESEQUENCE TO THE PACKET THAT HAS INCOME
    }
    if(!moveSequence) //IF MOVESEQUENCE(BUTTONS) ARE NOT PRESSED
    {
   
      steering.write(incomingRadioPacket.angle);//SET THE STEERING TO THE CONTROLLER
      speed = map(incomingRadioPacket.throttle,-100,100,0,180); // Convert -100 to 100 throttle settings to 0 to 180 degree servo values SO THE SERVO RECOGNISES IT
      throttle.write(speed);
    }  
    lastrx=millis(); //SET THE LAST TIME WE RECEIVED INFO FROM THE RADIO TO MILLIS (LAST TIME IN MILLISECONDS THE BOARD BEGAN RUNNING)
  }
}

void setup() {
  // Setup Throttle and Steering PWMs for servo control ...
  //MAKES IT SO IF NO INFO FROM CONTROLLER IS RECIEVED THROTTLE AND STEERING IS OFF (STEERING IS STRAIGHT)
  throttle.setPeriodHertz(50);
  throttle.attach(27, 1000, 2000);
  steering.setPeriodHertz(50);
  steering.attach(22, 1000, 2000);
  // Set them both for 90 degrees (which is 0 for throttle and centered for steering)
  throttle.write(90);
  steering.write(90);
  // Configure radio
  setupRadio(1,packetRx);
  Serial.println('Starting');
}

void move(int thisSteer, int thisSpeed, int thisMillis) // this is called for pre-programmed moves.  thisSteer=-100 (full left) to +100 (full right), thisSpeed=-100 (full back) to +100 (full forawrd).  thisMillis is how long to run for
{
  int i;
  if(!moveSequence)
   return;
  steering.write(map(thisSteer,-100,100,0,180));
  throttle.write(map(thisSpeed,-100,100,0,180));
  for(i=1; i<=thisMillis; i++)  
  {
    delay(1);
    if(!moveSequence)
      break;
  }
}

void loop() {

  if (moveSequence==1){

  }
  else if (moveSequence==2){

  }
  else if (moveSequence==3){
    
  }
  else if (moveSequence==4){
  move(0, 10, 30);
  move(0, 15, 30);
  move(0, 20, 30);
  move(0, 25, 30);
  move(0, 30, 30);
  move(0, 35, 30);
  move(0, 40, 30);
  move(0, 45, 30);
  move(0, 50, 30);
  move(0, 55, 30);
  move(0, 60, 30);
  move(0, 65, 30);
  move(0, 70, 30);
  move(0, 75, 30);
  move(0, 80, 30);
  move(0, 85, 30);
  move(0, 90, 30);
  move(0, 95, 30);
  move(0, 100, 30);
  moveSequence = 0;
    
  }
  else if (moveSequence==5){
    
    move(0, -25, 1);

    
  }
  


  
//  if(millis()>(lastrx+200)) // if not heard from the transmitter inthe last 200mS something might have gone wrong!
//  {
//    // ... so if going forward, gradually reduce the speed
//    if(speed>90)
//      speed-=2;
//    // ... and if going backwards, do the same
//    else if(speed<90)
//      speed+=2;
//    if((speed==89) || (speed==91)) // Just stops jittering 
//      speed=90;FRBG
//    throttle.write(speed);
//  }
//  delay(10);
}

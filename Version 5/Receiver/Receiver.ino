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
    if(incomingRadioPacket.command==6)   // button 6 pressed which should cancel move sequence if running
      moveSequence=0;
    else if(incomingRadioPacket.command) // button 1-5 pressed, so set the moveSequence to this number
      moveSequence=incomingRadioPacket.command;

    if(!moveSequence)
    {
      steering.write(incomingRadioPacket.angle);
      speed = map(incomingRadioPacket.throttle,-100,100,0,180); // Convert -100 to 100 throttle settings to 0 to 180 degree servo values
      throttle.write(speed);
    }  
    lastrx=millis();
  }
}

void setup() {
  // Setup Throttle and Steering PWMs for servo control ...
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
   move(0,-25,100);
  }
  else if (moveSequence==2){
    // button 2 code
    
  }
  else if (moveSequence==3){
    // button 3 code
    
  }
  else if (moveSequence==4){
    // button 4 code
    
  }
  else if (moveSequence==5){  
    // button 5 code
    
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
//      speed=90;
//    throttle.write(speed);
//  }
//  delay(10);
}

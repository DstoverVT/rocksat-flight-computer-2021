/*
 * RockSat-X 2021 Payload
 * DeploymentMotorControl.ino
 * Purpose: Control antenna deployment and retraction stepper motor with MSP432
 * Thread 2
 * 
 * Author: Danny Stover
 * Last Modified: 04/11/2022
 * Test status: COMPLETE (with mechanical integration)
 * 
 * Documentation: https://docs.google.com/document/d/1SsPBBd-b_qpj4nwn5OHS1XSpRm8F_LmyZ0yvp8VQmdE/edit?usp=sharing
 */

#include "MotorLib.h"

#define MILLIS 1000

const int RELAY_PIN = 4;                // MSP432 P3.3 for relay connected to network switch
const int TIMER_PIN = 78;               // MSP432 P1.0 connected to TE-R timer line
const int STEP_PIN = 39;                // MSP432 pin number for stepper motor STEP (P2.6 = Energia pin 39)
const int DIR_PIN = 40;                 // MSP432 pin number for stepper motor DIR (P2.7 = Energia pin 40)
const int PULLER_PIN = 32;              // MSP432 P3.5 for pin puller

const int RETRACTION_TIME = 300;        // Time (seconds) to wait AFTER deployment before retracting motor in seconds (dwell time of motor)
const int relayTime = 45;           // Seconds until turning relay on
const int pinPullTime = 5;          // Seconds for pulling pin

unsigned long timerEventTime = 0;
unsigned long pinPullerTimer = 0;
unsigned long relayTimer = 0;

bool deployed = false;                  // False = not deployed yet, True = deployment has occurred
bool retracted = false;                 // False = not retracted yet, True = retraction has occurred
bool deploymentMessageSent = false;
bool timerEvent = false;
bool relayToggled = false;
bool pinPulled = false;
bool pinPullTimer = false;


bool TERTriggered(bool firstTimer)
{
  return (digitalRead(TIMER_PIN) && firstTimer && timerCompare(timerEventTime, 1) && !timerEvent);
}


bool timerCompare(unsigned long startTime, unsigned long compareTime)
{
  unsigned long currTime = millis();
  return ((currTime - startTime) >= (compareTime * MILLIS));
}


void readyToDeploy()
{
  static bool firstTimer = false;
  if(digitalRead(TIMER_PIN) && !firstTimer)
  {
    timerEventTime = millis();
    firstTimer = true;
  }
  
  // Make sure timer is still high after one second for confirmation
  if(TERTriggered(firstTimer))
  {
    timerEventTime = millis();
    timerEvent = true;
  }
  else {
    firstTimer = false;
  }
}


void toggleRelay(bool relay)
{
  if(relay) {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else {
    digitalWrite(RELAY_PIN, LOW);
  }
}


void relayTrigger()
{
  unsigned long currTime = millis();
  // Toggle relay after 45 seconds
  if(timerCompare(relayTimer, relayTime) && !relayToggled) {
    toggleRelay(false);
    relayToggled = true;
  }
}


void pinPuller()
{
  static bool pullerSetHigh = false;
  unsigned long currTime = millis();
  if(!pinPulled && !pullerSetHigh) {
    digitalWrite(PULLER_PIN, HIGH);
    pullerSetHigh = true;
  }
  if(timerCompare(pinPullerTimer, pinPullTime) && !pinPulled) {
    pinPulled = true;
    // Turn pin puller off
    digitalWrite(PULLER_PIN, LOW); 
  }
}


void deploy()
{
  // Wait 10 seconds after timer event for pin puller
  static bool setDeployDirPin = false;
  if(timerEvent && pinPulled && !deployed)
  {
    if(!setDeployDirPin) {
      digitalWrite(DIR_PIN, LOW);  // Clockwise
      setDeployDirPin = true;
    }
    deployed = deployMotor();
  }
}


void retract()
{
  // Retract at T+300
  static bool setRetractDirPin = false;
  if(((flightTimeSeconds - RETRACTION_TIME) >= 0) && !retracted)
  {
    if(!setRetractDirPin) {
      digitalWrite(DIR_PIN, HIGH);  // Clockwise
      setRetractDirPin = true;
    }
    retracted = retractMotor();
  }
}


void setupMotor() 
{
//  Serial.begin(115200);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PULLER_PIN, OUTPUT);
  pinMode(TIMER_PIN, INPUT);

//  Serial.println("Deployment Thread Start");
  // Turn relay off
  toggleRelay(true);
  relayTimer = millis();
}


void loopMotor() 
{
//  Serial.print("deployed: ");
//  Serial.println(deployed);
//  Serial.print("retracted: ");
//  Serial.println(retracted);

  if(!deployed || !retracted)
  { 
    // Toggle relay after 45 seconds
    relayTrigger();
    // DEPLOY: After deployment timer (TE-R) received, step stepper motor
    readyToDeploy();

    // Pin puller before stepping motor
    if(timerEvent) {
      if(!pinPullTimer) {
        pinPullerTimer = millis();
        pinPullTimer = true;
      }
      pinPuller();
    }
    deploy();

    if(deployed) 
    {
      if(!deploymentMessageSent) {
//        Serial.println("Deployment Successful");
        messages += "Deployment Successful; ";
        deploymentMessageSent = true;
      }
      // RETRACT: After retraction time, step stepper motor in opposite direction
      retract();

      if(retracted) {
        messages += "Retraction Successful; ";
//        Serial.println("Deployment and Retraction finished");
      }
    }
  }  
  else
  {
//    Serial.println("Thread terminated");
    delay(30000);
  }
  
}

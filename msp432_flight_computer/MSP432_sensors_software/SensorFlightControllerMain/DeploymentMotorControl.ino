/*
 * RockSat-X 2021 Payload
 * DeploymentMotorControl.ino
 * Purpose: Control antenna deployment and retraction stepper motor with MSP432
 * 
 * Author: Danny Stover
 * Last Modified: 04/11/2021
 * Test status: COMPLETE (without integration with mechanical)
 * 
 * Documentation: https://docs.google.com/document/d/1SsPBBd-b_qpj4nwn5OHS1XSpRm8F_LmyZ0yvp8VQmdE/edit?usp=sharing
 */

#include "MotorLib.h"

unsigned long programTimer;             // Time since startup in seconds

const int STEP_PIN = 39;                // MSP432 pin number for stepper motor STEP (P2.6 = Energia pin 39)
const int DIR_PIN = 40;                 // MSP432 pin number for stepper motor DIR (P2.7 = Energia pin 40)

const int DEPLOY_TIME = 5;            // Time (seconds) to wait till deployment in seconds after electornics power up
const int RETRACTION_TIME = 5;        // Time (seconds) to wait AFTER deployment before retracting motor in seconds (dwell time of motor)

bool deployed = false;                  // False = not deployed yet, True = deployment has occurred
bool retracted = false;                 // False = not retracted yet, True = retraction has occurred


void setupMotor() 
{
  Serial.begin(115200);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  #ifdef DEBUG
  Serial.println("Deployment Thread Start");
  #endif
}


void loopMotor() 
{
  #ifdef DEBUG
  Serial.print("deployed: ");
  Serial.println(deployed);
  Serial.print("retracted: ");
  Serial.println(retracted);
  #endif
  if(!deployed && !retracted)
  {
    // AWAIT for DEPLOY_TIME seconds
    // Delay will sleep this thread and fully focus on others (sensor data)
    delay(DEPLOY_TIME * 1000);
    
    // DEPLOY: After deployment time, step stepper motor
    if(!deployed)
    {
      deployed = deployMotor();
      messages += "Deployment Successful; ";
    }
  
    // STANDBY for RETRACTION_TIME seconds after deployment
    delay(RETRACTION_TIME * 1000);
  
    // RETRACT: After retraction time, step stepper motor in opposite direction
    if(!retracted)
    {
      retracted = retractMotor();
      messages += "Retraction Successful; ";
    }
  
    // END of thread
    Serial.println("Deployment Thread Done");
  }  
  else
  {
    delay(30000);
  }
  
}

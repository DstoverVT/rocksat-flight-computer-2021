/*
 * MotorLib.cpp
 * RockSat-X 2022
 * 
 * Motor control implementation file.
 */

#include "MotorLib.h"

const int STEP_PIN = 39;                // MSP432 pin number for stepper motor STEP (P2.6 = Energia pin 39)
const int DIR_PIN = 40;                 // MSP432 pin number for stepper motor DIR (P2.7 = Energia pin 40)

const int stepsPerRevolution = 200;     // Stepper motor spec states 1.8 degree step angle, which equates to 200 steps per full revolution

const int DEPLOYMENT_REVOLUTIONS = 5.5;  // Number of revolutions required to deploy antenna fully
const int RETRACTION_REVOLUTIONS = 5.5;  // Number of revolutions required to retract antenna fully

const int MAX_DEPLOY_STEPS = DEPLOYMENT_REVOLUTIONS * stepsPerRevolution;
const int MAX_RETRACT_STEPS = RETRACTION_REVOLUTIONS * stepsPerRevolution;

const int STEP_DELAY = 8000;             // Delay between each step in microseconds (lower this number for a faster rotation)
int deploySteps = 0;
int retractSteps = 0;


bool deployMotor()
{
//    Serial.println("DEPLOYING");
    // Step stepper motor to deploy antenna
    
    if(deploySteps < MAX_DEPLOY_STEPS) {
      digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(STEP_DELAY);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(STEP_DELAY);
      ++deploySteps;
//      Serial.println(deploySteps);
    }
    else {
      return true;
    }

    return false;
}


bool retractMotor()
{
//    Serial.println("RETRACTING");    
    // Step stepper motor in reverse direction to retract antenna
    
    if(retractSteps < MAX_RETRACT_STEPS) {
      digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(STEP_DELAY);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(STEP_DELAY);
      ++retractSteps;
//      Serial.println(retractSteps);
    }
    else {
      return true;
    }

    return false;
}

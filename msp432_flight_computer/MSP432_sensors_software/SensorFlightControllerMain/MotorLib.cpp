#include "MotorLib.h"

const int STEP_PIN = 39;                // MSP432 pin number for stepper motor STEP (P2.6 = Energia pin 39)
const int DIR_PIN = 40;                 // MSP432 pin number for stepper motor DIR (P2.7 = Energia pin 40)

const int stepsPerRevolution = 200;     // Stepper motor spec states 1.8 degree step angle, which equates to 200 steps per full revolution

const int DEPLOYMENT_REVOLUTIONS = 2;  // Number of revolutions required to deploy antenna fully
const int RETRACTION_REVOLUTIONS = 2;  // Number of revolutions required to retract antenna fully

const int STEP_DELAY = 1000;             // Delay between each step in microseconds (lower this number for a faster rotation)

bool deployMotor()
{
    Serial.println("DEPLOYING");
    // Step stepper motor to deploy antenna
    digitalWrite(DIR_PIN, HIGH);  // Clockwise
    for(int step = 0; step < DEPLOYMENT_REVOLUTIONS * stepsPerRevolution; step++) {
      digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(STEP_DELAY / 2);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(STEP_DELAY / 2);
    }
    Serial.println("DEPLOYMENT DONE");

    return true;
}

bool retractMotor()
{
    Serial.println("RETRACTING");    
    // Step stepper motor in reverse direction to retract antenna
    digitalWrite(DIR_PIN, LOW);  // Counter-Clockwise
    for(int step = 0; step < RETRACTION_REVOLUTIONS * stepsPerRevolution; step++) {
      digitalWrite(STEP_PIN, HIGH); 
      delayMicroseconds(STEP_DELAY / 2);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(STEP_DELAY / 2);
    }
    Serial.println("RETRACTION DONE");

    return true;
}

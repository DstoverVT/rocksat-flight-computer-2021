/*
 * MotorLib.h
 * RockSat-X 2022
 * 
 * Motor control definition file.
 */

#ifndef MOTORLIB_H
#define MOTORLIB_H

#include "Energia.h"
#include "DataConfig.h"

// Integers that hold current step motor is on
extern int deploySteps;
extern int retractSteps;

// Functions to deploy and retract stepper motor
bool deployMotor();
bool retractMotor();

#endif  /* MOTORLIB_H */

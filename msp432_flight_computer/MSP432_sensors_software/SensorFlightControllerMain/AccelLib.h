#ifndef ACCELLIB_H
#define ACCELLIB_H

#include "SDLib.h"

// ADXL sensor variables
extern int range;
extern int X_PIN;
extern int Y_PIN;
extern int Z_PIN;

// Configure IMU Peripheral
void init_Accel();

// Prints IMU data to Serial monitor
void printAccelData(float accelX, float accelY, float accelZ);

// Saves IMU data to SD card file
void storeAccelData(char* fileName, float accelX, float accelY, float accelZ);

// Collects accelerometer, gyroscope, temperature, and magnetometer data from ICM-20948 IMU
void handleAccelData(char* fileName);

// Convert analog reading into acceleration
float readingToAcceleration(float reading);

#endif  /* ACCELLIB_H */

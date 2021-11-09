#ifndef IMULIB_H
#define IMULIB_H

#include "SDLib.h"
#include <ICM_20948.h>
#include <Wire.h>

#define AD0_VAL     1
#define TO_GRAVITY  9.8

extern ICM_20948_I2C ICM_sensor;

// Configure IMU Peripheral
void init_IMU();

// Helper functions for init_IMU
// Return true if IMU communications set up successfully; false otherwise
bool setup_IMU_I2C();
void configureIMU();

// Prints IMU data to Serial monitor
void printIMUData();

// Saves IMU data to SD card file
void storeIMUData(char* fileName);

// Collects accelerometer, gyroscope, temperature, and magnetometer data from ICM-20948 IMU
void handleIMUData(char* fileName);

void printICMStatusWithMessage(char* message);

#endif  /* IMULIB_H */

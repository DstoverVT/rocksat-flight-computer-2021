#ifndef BAROMETERLIB_H
#define BAROMETERLIB_H

#include "SDLib.h"
#include <BMP388_DEV.h>  

extern BMP388_DEV bmp388; 

// Configure IMU Peripheral
void init_Barometer();

// Prints IMU data to Serial monitor
void printBarometerData(float temperature, float pressure, float altitude);

// Saves IMU data to SD card file
void storeBarometerData(char* fileName, float temperature, float pressure, float altitude);

// Collects accelerometer, gyroscope, temperature, and magnetometer data from ICM-20948 IMU
void handleBarometerData(char* fileName);

#endif  /* BAROMETERLIB_H */

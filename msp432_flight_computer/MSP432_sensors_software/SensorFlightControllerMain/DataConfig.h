/*
 * DataConfig.h
 * RockSat-X 2022
 * 
 * Data config definition file.
 */

#ifndef DATACONFIG_H
#define DATACONFIG_H
//#define DEBUG 1

#include "Energia.h"

#define PERIPHERAL_NUM  3   // IMU, ACCEL, BAROMETER

enum Peripheral
{ IMU, ACCEL, BAROMETER };
// 0: IMU
// 1: ACCEL
// 2: BAROMETER

// Array indices match those in Peripheral enum, minus the SD card
extern bool printData[PERIPHERAL_NUM];
extern bool saveToSD[PERIPHERAL_NUM];
extern String messages;

// Print all sensors to Serial monitor
void printAllData();

// Don't print any sensor data to Serial monitor
void printNoData();

// Save all sensor data to SD card in respective files
void saveAllDataToSD();

// Don't save any data to SD card
void saveNoDataToSD();

// Print a specific sensor's data only
void printSensor(Peripheral sensor);

// Save a specific sensor's data to the SD card only
void saveSensorToSD(Peripheral sensor);

#endif  /* DATACONFIG_H */

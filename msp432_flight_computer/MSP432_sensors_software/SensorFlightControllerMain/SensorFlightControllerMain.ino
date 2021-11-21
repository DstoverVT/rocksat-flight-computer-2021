/*
 * SensorFlightControllerMain.ino
 * RockSat-X 2021
 * 
 * Written by: Danny Stover
 * Last updated: 08/14/2021
 */

#include "DataConfig.h"
#include "SDLib.h"
#include "IMULib.h"
#include "AccelLib.h"
#include "BarometerLib.h"

#define ELECTRONICS_STARTUP -180.0 // Seconds before launch that electronics start up
#define FILE_NUM_LOCATION   7

// Program configurable parameters
char sensorFileName[] = "sensors0.csv";                // File name (must be 8 characters or less before .csv), increments number at end if already exists (up to 9)
uint8_t fileNumberIndex = FILE_NUM_LOCATION;  // Index of file name number (right before .csv)

// Time variables
float flightTimeSeconds;                               // Keeps track of time since launch (electronics should start at T-180)


// Helper function to convert millis to seconds
float milliToSeconds(unsigned long milliTime)
{
  return (float)(milliTime / 1000.0);
}


// Initializes all hardware
void setupMain() 
{
  Serial.begin(115200);
  #ifdef DEBUG
  Serial.println("----------------MAIN PROGRAM START---------------");
  #endif

  // Data output configuration
  printAllData();
  saveAllDataToSD();

  SD_initialized = init_SD();

  // Create file on SD card called sensorFileName
  // Automatically renumbers duplicate files from 0 - 9 (MUST delete/rename a file after 10 are stored on SD card with consecutive numbers)
  createFile(sensorFileName, fileNumberIndex);

  #ifdef DEBUG
  Serial.println("init IMU from main");
  #endif
  IMU_initialized = init_IMU();
  #ifdef DEBUG
  Serial.println("init Accel from main");
  #endif
  init_Accel();
  #ifdef DEBUG
  Serial.println("init Barometer from main");
  #endif
  BMP_initialized = init_Barometer();
}


// Main flight program loop
void loopMain() 
{ 
//  // Calculate time since launch
  flightTimeSeconds = ELECTRONICS_STARTUP + milliToSeconds(millis());
  #ifdef DEBUG
  Serial.print("Time: ");
  Serial.println(flightTimeSeconds);
  #endif
//
//  // Write time to SD card file
  writeTimestamp(sensorFileName, flightTimeSeconds);

  // Handle sensor data collection and SD storage
  handleIMUData(sensorFileName);
  Serial.println();
  handleAccelData(sensorFileName);
  Serial.println();
  handleBarometerData(sensorFileName);
  Serial.println();

  // Write message to SD card file 
  writeMessage(sensorFileName);  
  
  delay(500);
}

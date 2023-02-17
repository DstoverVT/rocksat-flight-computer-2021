/*
 * SensorFlightControllerMain.ino
 * RockSat-X 2022
 * 
 * Written by: Danny Stover
 * Last updated: 06/04/2022
 * 
 * Thread 1 (Data collection): This file creates a thread that executes sensor 
 * initialization and executes the handling of sensor data.
 */

#include "DataConfig.h"
#include "SDLib.h"
#include "IMULib.h"
#include "AccelLib.h"
#include "BarometerLib.h"

#define LED 31                                      // Test LED to ensure script is uploaded (although the program could still be hung somewhere). P3.7 on MSP432
#define ELECTRONICS_STARTUP -180.0                  // Seconds before launch that electronics start up
#define FILE_NUM_LOCATION   7

// Program configurable parameters
char sensorFileName[] = "sensors0.csv";             // File name (must be 8 characters or less before .csv), increments number at end if already exists (up to 9)
const uint8_t fileNumberIndex = FILE_NUM_LOCATION;  // Index of file name number (right before .csv)

// Time variables
float flightTimeSeconds;                            // Keeps track of time since launch (electronics should start at T-180)


// Helper function to convert millis to seconds
float milliToSeconds(unsigned long milliTime)
{
  return (float)(milliTime / 1000.0);
}


// Initializes all hardware
void setupMain() 
{
//  Serial.begin(115200);
  pinMode(LED, OUTPUT); 
//  Serial.println("----------------MAIN PROGRAM START---------------");

  // Data output configuration
  printNoData();
  saveAllDataToSD();

  digitalWrite(LED, HIGH);            // Test LED 

  SD_initialized = init_SD();

  // Create file on SD card called sensorFileName
  // Automatically renumbers duplicate files from 0 - 9 (MUST delete/rename a file after 10 are stored on SD card with consecutive numbers)
  createFile(sensorFileName, fileNumberIndex);

//  Serial.println("init IMU from main");
  IMU_initialized = init_IMU(); 
//  Serial.println("init Accel from main");
  init_Accel();
//  Serial.println("init Barometer from main");
  BMP_initialized = init_Barometer();
}


// Main flight program loop
void loopMain() 
{ 
  // Calculate time since launch
  flightTimeSeconds = ELECTRONICS_STARTUP + milliToSeconds(millis());
//  Serial.print("Time: ");
//  Serial.println(flightTimeSeconds);

  // Write time to SD card file
  writeTimestamp(sensorFileName, flightTimeSeconds);

  // Handle sensor data collection and SD storage
  handleIMUData(sensorFileName);
//  Serial.println();
  handleAccelData(sensorFileName);
//  Serial.println();
  handleBarometerData(sensorFileName);
//  Serial.println();

  // Write message to SD card file 
  writeMessage(sensorFileName);  

  // Sensor data collects at ~1-2 Hz.
  delay(500);
}

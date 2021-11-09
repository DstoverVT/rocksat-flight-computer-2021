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
unsigned long milliseconds;
float flightTimeSeconds;                               // Keeps track of time since launch (electronics should start at T-180)


// Helper function to convert millis to seconds
float milliToSeconds(unsigned long milliTime)
{
  return (float)(milliTime / 1000.0);
}


// Initializes all hardware
void setup() 
{
  Serial.begin(115200);
  Serial.println("----------------PROGRAM START---------------");

  // Data output configuration
  printAllData();     /* Choices for Serial print (from DataConfig.h):
                         printAllData - prints data for all sensors
                         printNoData - prints data for no sensors
                         printSensor - prints data for a single sensor
                      */
  saveNoDataToSD();  /* Choices for SD card saving (from DataConfig.h):
                         saveAllDataToSD
                         saveNoDataToSD
                         saveSensorToSD
                      */

  init_SD();

  // Create file on SD card called sensorFileName
  // Automatically renumbers duplicate files from 0 - 9 (MUST delete/rename a file after 10 are stored on SD card with consecutive numbers)
  createFile(sensorFileName, fileNumberIndex);

  Serial.println("init IMU");
  init_IMU();
  Serial.println("init Accel");
  init_Accel();
  Serial.println("init Barometer");
  init_Barometer();
}


// Main flight program loop
void loop() 
{ 
  // Calculate time since launch
  flightTimeSeconds = ELECTRONICS_STARTUP + milliToSeconds(millis());
  Serial.print("Time: ");
  Serial.println(flightTimeSeconds);

  // Write time to SD card file
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
  
  delay(1000);
}

/*
 * AccelLib.cpp
 * RockSat-X 2022
 * 
 * Accelerometer implementation file.
 */

#include "AccelLib.h"

// ADXL sensor variables
const int range = 200;
const int X_PIN = A3;
const int Y_PIN = A4;
const int Z_PIN = A2;


// Configure IMU Peripheral
void init_Accel()
{
  // 14 bit ADC resolution from MSP432
  analogReadResolution(14);
}


// Prints accelerometer data to Serial monitor
void printAccelData(float accelX, float accelY, float accelZ)
{
  Serial.println("------------- ACCELEROMETER ---------------");
  Serial.print(F("accelX: "));
  Serial.print(accelX);                      
  Serial.print(F("g   "));

  Serial.print(F("accelY: "));
  Serial.print(accelY);    
  Serial.print(F("g   "));

  Serial.print(F("accelZ: "));
  Serial.print(accelZ);
  Serial.println(F("g")); 
}


// Saves accelerometer data to SD card file
void storeAccelData(char* fileName, float accelX, float accelY, float accelZ)
{
  sensor_file = SD.open(fileName, FILE_WRITE);
  // Write data to ADXL csv file
  if(sensor_file) {
    sensor_file.print(',');
    sensor_file.print(accelX);
    sensor_file.print(',');
    sensor_file.print(accelY);
    sensor_file.print(',');
    sensor_file.print(accelZ);

    sensor_file.close();
  }
  else {
    messages += "ADXL SD file could not be opened/found, program will continue without writing data to the SD card; ";    
    saveToSD[ACCEL] = false;
  }
}


// Collects accelerometer, gyroscope, temperature, and magnetometer data from ICM-20948 IMU
void handleAccelData(char* fileName)
{
//  Serial.println("Handling Accel data");

  int accelX_raw = analogRead(X_PIN);
  int accelY_raw = analogRead(Y_PIN);
  int accelZ_raw = analogRead(Z_PIN);

  float accelX = readingToAcceleration(accelX_raw);
  float accelY = readingToAcceleration(accelY_raw);
  float accelZ = readingToAcceleration(accelZ_raw);

//  if(printData[ACCEL]) {
//    printAccelData(accelX, accelY, accelZ);
//  }

  if(saveToSD[ACCEL]) {
    storeAccelData(fileName, accelX, accelY, accelZ);
  }
}


float readingToAcceleration(float reading)
{
  int maxAnalogRead = pow(2, 14) - 1;               // 14 bit ADC
//  int maxInput = (3.3 / 5.0) * maxAnalogRead;     // 3.3V maximum
  int maxInput = maxAnalogRead;                     // 5V maximum
  float readingRatio = reading / (float)maxInput;
  return readingRatio * (range * 2.0) - range;      // Convert reading ratio to acceleration (in g's)
}

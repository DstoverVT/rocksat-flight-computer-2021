#include "DataConfig.h"

// Prints and stores no data by default, designed for global access
bool printData[PERIPHERAL_NUM] = { false, false, false };
bool saveToSD[PERIPHERAL_NUM] = { false, false, false };

// Message log
String messages = "";

// Configure which sensor data to print to Serial monitor
void printAllData()
{
  for(int i = 0; i < PERIPHERAL_NUM; i++) {
    printData[i] = true;
  }
}


void printNoData()
{
  for(int i = 0; i < PERIPHERAL_NUM; i++) {
    printData[i] = false;
  }
}


void printSensor(Peripheral sensor)
{
  printData[sensor] = true;
}


// Configure which sensor data to save to SD card
void saveAllDataToSD()
{
  for(int i = 0; i < PERIPHERAL_NUM; i++) {
    saveToSD[i] = true;
  }
}


void saveNoDataToSD()
{
  for(int i = 0; i < PERIPHERAL_NUM; i++) {
    saveToSD[i] = false;
  }
}


void saveSensorToSD(Peripheral sensor)
{
  saveToSD[sensor] = true;
}

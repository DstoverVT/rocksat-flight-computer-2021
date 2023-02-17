/*
 * SensorInitializationDebug.ino
 * RockSat-X 2022
 * 
 * Written by: Danny Stover
 * Last updated: 06/04/2022
 * 
 * Thread 3 (Debug thread): This file creates a thread that debugs sensors.
 * Attempts to initialize sensors that don't initialize at first.
 */

#include "SDLib.h"
#include "IMULib.h"
#include "BarometerLib.h"

const int wait_time = 2000;


void setupDebug()
{   
//    Serial.begin(115200);
    delay(3000);    // Give other threads time to initialize

//    Serial.println("Sensor Debug Thread Start");
}

void loopDebug()
{
    // Serial.println("Searching for sensors to debug..");
    if(!IMU_initialized)
    {
//        Serial.println("ATTEMPT: Reinitializing IMU");
        messages += "DEBUG: Attempting to reinitialize IMU; ";  
        IMU_initialized = init_IMU();        
    }
    
    if(!BMP_initialized)
    {
//        Serial.println("ATTEMPT: Reinitializing BMP");
        messages += "DEBUG: Attempting to reinitialize BMP; ";  
        BMP_initialized = init_Barometer();
    }
    
    if(!SD_initialized)
    {
//        Serial.println("ATTEMPT: Reinitializing SD");
        messages += "DEBUG: Attempting to reinitialize SD; ";  
        SD_initialized = init_SD();
        if(SD_initialized) {
            if(IMU_initialized) {
              saveToSD[IMU] = true;
            }
            if(BMP_initialized) {
              saveToSD[BAROMETER] = true;
            }
            saveToSD[ACCEL] = true;
            createFile(sensorFileName, fileNumberIndex);
        }
    }

    delay(wait_time);
}

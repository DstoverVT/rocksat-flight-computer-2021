#include "SDLib.h"
#include "IMULib.h"
#include "BarometerLib.h"

const int wait_time = 2000;

void setupDebug()
{   
    Serial.begin(115200);
    delay(1000);    // Give other threads time to initialize

    #ifdef DEBUG
    Serial.println("Sensor Debug Thread Start");
    #endif
}

void loopDebug()
{
    // Serial.println("Searching for sensors to debug..");
    if(!IMU_initialized)
    {
        #ifdef DEBUG
        Serial.println("ATTEMPT: Reinitializing IMU");
        #endif
        IMU_initialized = init_IMU();        
    }
    
    if(!BMP_initialized)
    {
        #ifdef DEBUG
        Serial.println("ATTEMPT: Reinitializing BMP");
        #endif
        BMP_initialized = init_Barometer();
    }
    
    if(!SD_initialized)
    {
        #ifdef DEBUG
        Serial.println("ATTEMPT: Reinitializing SD");
        #endif
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

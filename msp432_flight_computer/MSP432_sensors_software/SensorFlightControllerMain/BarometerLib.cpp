#include "BarometerLib.h"

BMP388_DEV bmp388; 
bool bmp_ready = false;

// Configure IMU Peripheral
void init_Barometer()
{
    // Initialization according to datasheet recommendation for drone (similar to rocket payload)
    bmp_ready = bmp388.begin(SLEEP_MODE,                        // Start in sleep mode (no measurements)
                            OVERSAMPLING_X8,                   // Pressure sensor oversampling (x8)
                            OVERSAMPLING_SKIP,                 // Temperatuer sensor oversampling (x1)
                            IIR_FILTER_2,                      // IIR filter value (2) = more sensitive to sudden impulses
                            TIME_STANDBY_20MS);                // Output data rate period = 20ms

    if(!bmp_ready) {
      messages = "Barometer BMP388 communication setup failed; ";   
      saveToSD[BAROMETER] = false;
      printData[BAROMETER] = false;
    }
    else {
      bmp388.startNormalConversion();                 // Start BMP388 continuous conversion in NORMAL_MODE 
      messages += "Barometer BMP388 communication setup complete; ";    
    }
}


// Prints IMU data to Serial monitor
void printBarometerData(float temperature, float pressure, float altitude)
{
    Serial.println("------------- BAROMETER ---------------");
    Serial.print(F("Temperature: "));
    Serial.print(temperature);                    
    Serial.print(F("*C   "));

    Serial.print(F("Pressure: "));
    Serial.print(pressure);    
    Serial.print(F("hPa   "));

    Serial.print(F("Altitude: "));
    Serial.print(altitude);
    Serial.println(F("m")); 
}


// Saves IMU data to SD card file
void storeBarometerData(char* fileName, float temperature, float pressure, float altitude)
{
  sensor_file = SD.open(fileName, FILE_WRITE);
  // Write data to BMP csv file
  if(sensor_file) {
    sensor_file.print(',');
    sensor_file.print(temperature);
    sensor_file.print(',');
    sensor_file.print(pressure);
    sensor_file.print(',');
    sensor_file.print(altitude);
    
    sensor_file.close();
  }
  else {
    messages += "BMP SD file could not be opened/found, program will continue without writing data to the SD card; ";    
    saveToSD[BAROMETER] = false;
  }
}


// Collects accelerometer, gyroscope, temperature, and magnetometer data from ICM-20948 IMU
void handleBarometerData(char* fileName)
{
  if(bmp_ready) {
    float temperature, pressure, altitude;            // Create the temperature, pressure and altitude variables

    if (bmp388.getMeasurements(temperature, pressure, altitude))    // Check if the measurement is complete
    {
        if(printData[BAROMETER]) {
            printBarometerData(temperature, pressure, altitude);
        }

        if(saveToSD[BAROMETER]) {
            storeBarometerData(fileName, temperature, pressure, altitude);
        }
    }
  }
}
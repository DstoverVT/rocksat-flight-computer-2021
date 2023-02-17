/*
 * SDlib.cpp
 * RockSat-X 2022
 * 
 * SD implementation file.
 */

#include "SDLib.h"

// Global access to file
File sensor_file;
bool SD_initialized = false;
bool sd_ready = false;

// Uses SD.begin to initialize SD card
bool init_SD()
{
//    Serial.println("SD startup - before begin");
    sd_ready = SD.begin(SDCardChipSelect);
//    Serial.println("SD startup - after begin");

    if(sd_ready) {
//        Serial.println("SD startup succeeded");
        messages += "SD card reader initialization successful; "; 
        return true;       
    }
    else {
//        Serial.println("SD startup failed");
        messages += "SD card reader initialization failed, continuing without SD card; ";        
        saveNoDataToSD();   // Can't save any data to SD card
        return false;
    }
}


void createFile(char* fileName, uint8_t numIndex)
{
    if(SD_initialized) {
        // If file already exists, increment number at end
        while(SD.exists(fileName)) {
            char fileNum = fileName[numIndex];
            // Make sure this index is a digit
            if(isdigit(fileNum)) {                
                if(fileNum == '9') {
//                    Serial.println("10 files with this name have been created, will overwrite 0");                    
                    fileName[numIndex] = '0';                    
                    break;
                }        
                fileName[numIndex] = fileNum + 1; // Increment file name number by 1
            }
            // Make it a digit if it is not already one
            else {
                fileName[numIndex] = '0';
            }
        }

        // Create file
        SD.remove(fileName);
        sensor_file = SD.open(fileName, FILE_WRITE);

        // File opened successfully
        if(sensor_file) {
            // Write headers to file with Timestamp at beginning and Message at end
            // [m#] is just for simplicity of designating different measurements
            // (unit) is for units        
            sensor_file.print("Timestamp, ");
            if(saveToSD[IMU]) {
                sensor_file.print("IMU_Temperature, IMU_Accel_X, IMU_Accel_Y, IMU_Accel_Z, IMU_Mag_X, IMU_Mag_Y, IMU_Mag_Z, IMU_Gyro_X, IMU_Gyro_Y, IMU_Gyro_Z, ");
            }
            if(saveToSD[ACCEL]) {
                sensor_file.print("ADXL_Accel_X, ADXL_Accel_Y, ADXL_Accel_Z, ");
            }
            if(saveToSD[BAROMETER]) {
                sensor_file.print("BMP_Temperature, BMP_Pressure, BMP_Altitude, ");
            }        
            sensor_file.println("Message");        

            sensor_file.close();  // Save SD card file
//            Serial.print("SD card file created with name: ");
//            Serial.println(fileName);
            messages += "SD card file successfully created with the name: ";
            messages += fileName;
            messages += "; ";          
        }
        // File did not open successfully
        else {
//            Serial.println("SD card file could not be found");
            messages += "createFile: SD file could not be opened/found, program will continue without writing data to the SD card; ";            
            saveNoDataToSD();
        }
    }
    
}


void writeTimestamp(char* fileName, float time)
{ 
    if(SD_initialized) {   
        sensor_file = SD.open(fileName, FILE_WRITE);
        if(sensor_file) {
            sensor_file.print(time);
        }
        sensor_file.close();
    }
}


void writeMessage(char* fileName)
{   
    // Write messages to SD card    
    // If message is empty
    if(SD_initialized) {
        String sd_message = messages;
        if(sd_message.length() == 0) {
            sd_message = "No messages";
        }

        sensor_file = SD.open(fileName, FILE_WRITE);
        if(sensor_file) {
            sensor_file.print(',');
            sensor_file.print('[');
            sensor_file.print(sd_message);
            sensor_file.println(']');
        } 

        sensor_file.close();
    }

    // Output messages to console    
//    Serial.print("PRINTING MESSAGES: ");
//    // If message is empty
//    if(messages.length() == 0) {
//        messages = "No messages";
//    }
//
//    Serial.print("Messages: [");
//    Serial.print(messages);
//    Serial.println("]");
//    Serial.println();        

    messages = "";
}

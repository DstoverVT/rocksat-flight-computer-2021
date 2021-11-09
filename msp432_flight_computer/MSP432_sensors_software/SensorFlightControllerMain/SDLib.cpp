#include "SDLib.h"

// Global access to file
File sensor_file;
bool sd_ready = false;

// Uses SD.begin to initialize SD card
void init_SD()
{
    sd_ready = SD.begin(SDCardChipSelect);
    sd_ready = false;   // Print no data to SD card
    if(sd_ready) {
        messages += "SD Card communication setup complete; ";        
    }
    else {
        messages += "INIT_SD: Communication with SD Card failed, program will continue without SD card; ";        
        saveNoDataToSD();   // Can't save any data to SD card
    }
}


void createFile(char* fileName, uint8_t numIndex)
{
    if(sd_ready) {
        // If file already exists, increment number at end
        while(SD.exists(fileName)) {
            char fileNum = fileName[numIndex];
            // Make sure this index is a digit
            if(isdigit(fileNum)) {
                if(fileNum == '9') {
                    Serial.println("10 files with this name have been created, delete or rename one of them to create this one");
                }
                fileName[numIndex] = fileNum + 1; // Increment file name number by 1
            }
            // Make it a digit if it is not already one
            else {
                fileName[numIndex] = '1';
            }
        }

        // Create file
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
            messages += "SD card file successfully created with the name: ";
            messages += fileName;
            messages += "; ";          
        }
        // File did not open successfully
        else {
            messages += "createFile: SD file could not be opened/found, program will continue without writing data to the SD card; ";            
            saveNoDataToSD();
        }
    }
    
}


void writeTimestamp(char* fileName, float time)
{ 
    if(sd_ready) {   
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
    if(sd_ready) {
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
    Serial.print("PRINTING MESSAGES:");
    // If message is empty
    if(messages.length() == 0) {
        messages = "No messages";
    }

    Serial.print("Messages: [");
    Serial.print(messages);
    Serial.println("]");
    Serial.println();        

    messages = "";
}

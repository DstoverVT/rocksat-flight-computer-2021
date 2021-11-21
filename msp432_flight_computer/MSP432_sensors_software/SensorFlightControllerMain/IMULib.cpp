#include "IMULib.h"

ICM_20948_I2C ICM_sensor;
bool IMU_initialized = true;
bool imu_ready = false;


bool init_IMU()
{

    Wire.begin();

    bool initialized = false;
    ICM_sensor.begin( Wire, AD0_VAL );

    #ifdef DEBUG
    Serial.print( F("Initialization of the sensor returned: ") );
    Serial.println( ICM_sensor.statusString() );
    #endif
    if( ICM_sensor.status != ICM_20948_Stat_Ok ){
      Serial.print(F("Begin returned: "));
      Serial.println(ICM_sensor.statusString());    
    }
    else {
      initialized = true;
    }
    
    ICM_sensor.swReset( );
    if( ICM_sensor.status != ICM_20948_Stat_Ok){
      Serial.print(F("Software Reset returned: "));
      Serial.println(ICM_sensor.statusString());
    }
    delay(250);

    // Now wake the sensor up
    ICM_sensor.sleep( false );
    ICM_sensor.lowPower( false );

    // The next few configuration functions accept a bit-mask of sensors for which the settings should be applied.

    // Set Gyro and Accelerometer to a particular sample mode
    // options: ICM_20948_Sample_Mode_Continuous
    //          ICM_20948_Sample_Mode_Cycled
    ICM_sensor.setSampleMode( (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), ICM_20948_Sample_Mode_Continuous );
    if( ICM_sensor.status != ICM_20948_Stat_Ok){
      Serial.print(F("setSampleMode returned: "));
      Serial.println(ICM_sensor.statusString());
    }

    ICM_20948_fss_t myFSS;  // This uses a "Full Scale Settings" structure that can contain values for all configurable sensors

    myFSS.a = gpm16;         // (ICM_20948_ACCEL_CONFIG_FS_SEL_e)
                            // gpm2
                            // gpm4
                            // gpm8
                            // gpm16

    myFSS.g = dps2000;       // (ICM_20948_GYRO_CONFIG_1_FS_SEL_e)
                            // dps250
                            // dps500
                            // dps1000
                            // dps2000

    ICM_sensor.setFullScale( (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS );
    if( ICM_sensor.status != ICM_20948_Stat_Ok){
      Serial.print(F("setFullScale returned: "));
      Serial.println(ICM_sensor.statusString());
    }

    // Choose whether or not to start the magnetometer
    ICM_sensor.startupMagnetometer();
    if( ICM_sensor.status != ICM_20948_Stat_Ok){
      Serial.print(F("startupMagnetometer returned: "));
      Serial.println(ICM_sensor.statusString());
    }

    // If IMU errors occur
    if( ICM_sensor.status != ICM_20948_Stat_Ok) {
        #ifdef DEBUG
        Serial.println("IMU initialization failed");
        #endif
        messages += "IMU initialization failed, continuing without IMU; ";        
        saveToSD[IMU] = false;
        printData[IMU] = false;
        return false;
    }
    else {
        #ifdef DEBUG
        Serial.println("IMU initialization succeeded");
        #endif
        messages += "IMU initialization successful; ";        
        if(SD_initialized) {
          saveToSD[IMU] = true;
        }
        printData[IMU] = true;
        return true;
    }
}


void printIMUData()
{
    Serial.println("------------- IMU ---------------");
        /* Display temperature results (temperature is measured in deg C) */
    Serial.print("Temperature\t ");
    Serial.print(ICM_sensor.temp());
    Serial.println(" deg C");
  
    /* Display accelerometer results (acceleration is measured in m/s^2) */
    Serial.print("Accel\t X: ");
    Serial.print(ICM_sensor.accX());
    Serial.print(" \tY: ");
    Serial.print(ICM_sensor.accY());
    Serial.print(" \tZ: ");
    Serial.print(ICM_sensor.accZ());
    Serial.println(" g ");
  
    /* Display magnetometer results (magnetometer data is measured in uT) */
    Serial.print("Mag\t X: ");
    Serial.print(ICM_sensor.magX());
    Serial.print(" \tY: ");
    Serial.print(ICM_sensor.magY());
    Serial.print(" \tZ: ");
    Serial.print(ICM_sensor.magZ());
    Serial.println(" uT");
  
    /* Display gyro results (gyro data is measured in radians/s) */
    Serial.print("Gyro\t X: ");
    Serial.print(ICM_sensor.gyrX());
    Serial.print(" \tY: ");
    Serial.print(ICM_sensor.gyrY());
    Serial.print(" \tZ: ");
    Serial.print(ICM_sensor.gyrZ());
    Serial.println(" radians/s ");
    Serial.println();
}


void storeIMUData(char* fileName)
{
    sensor_file = SD.open(fileName, FILE_WRITE);
    // Write data to IMU csv file
    if(sensor_file) {
      // Timestamp comes before IMU data
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.temp());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.accX());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.accY());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.accZ());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.magX());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.magY());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.magZ());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.gyrX());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.gyrY());
      sensor_file.print(',');
      sensor_file.print(ICM_sensor.gyrZ());

      sensor_file.close();
    }
    else {
        messages += "IMU SD file could not be opened/found, program will continue without writing IMU data to the SD card; ";       
        saveToSD[IMU] = false;
    }
}


void handleIMUData(char* fileName)
{  
  #ifdef DEBUG
  Serial.println("Handling IMU data");
  Serial.println(ICM_sensor.statusString());
  #endif
  
  if(IMU_initialized) {
    // Collect IMU sensor data
    if(ICM_sensor.dataReady()) {
      #ifdef DEBUG
      Serial.println("Getting new IMU data");
      Serial.println(ICM_sensor.statusString());
      #endif
      ICM_sensor.getAGMT();   // AGMT = accelerometer, gyroscope, magnetometer, temperature. Updates readings

      // Print IMU sensor data (if printData is true)
      if(printData[IMU]) {
        printIMUData();
      }
  
      // Store IMU sensor data to SD card (if saveToSD is true)
//      if(saveToSD[IMU]) {
//        storeIMUData(fileName);
//      }
      
      #ifdef DEBUG
      Serial.println("Finished getting new IMU data");
      Serial.println(ICM_sensor.statusString());
      #endif
    }
  }
}


void printICMStatusWithMessage(char* message) {
    if(ICM_sensor.status != ICM_20948_Stat_Ok){
    Serial.print(message);
    Serial.println(ICM_sensor.statusString());
  }
}

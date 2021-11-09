#include "IMULib.h"

ICM_20948_I2C ICM_sensor;
bool imu_ready = false;

void init_IMU()
{
    // Set up I2C communication
    imu_ready = setup_IMU_I2C();

    // If IMU errors occur
    if(!imu_ready) {
        messages += "Tried to initialized IMU 100 times but failed, continuing without IMU; ";        
        saveToSD[IMU] = false;
        printData[IMU] = false;
    }
    else {
        messages += "IMU ICM-20948 communication setup complete; ";        
        configureIMU();
    }
}


bool setup_IMU_I2C()
{
    // *****FIX SO IMU WORKS IF NOT PLUGGED IN 
    Serial.println("Tried starting IMU Wire");   
    Wire.begin(); // MSP432 defaults to 400 kHz according to msp432r-core/cores/msp432r/ti/runtime/wiring/Wire.cpp in Energia GitHub (search for I2C_Params)
    
    bool icm_error = false;
    bool ICM_initialized = false;
    int ICM_initialization_attempts = 0;
    while(!icm_error && !ICM_initialized && ICM_initialization_attempts < 10) {
      // Keep trying to initialize I2C communication with ICM-20948
      ICM_sensor.begin(Wire, AD0_VAL);
      Serial.println("Tried starting IMU");
      if(ICM_sensor.status == ICM_20948_Stat_Ok) {      
        ICM_initialized = true;        
      }
      
      ICM_initialization_attempts++;
      delay(50);
    }

    return ICM_initialized;
}


void configureIMU()
{
        // Configure ICM-20948 sensor settings
    ICM_sensor.setSampleMode( (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), ICM_20948_Sample_Mode_Continuous );  // Set gyro and accelerometer sample modes to continuous
    // printICMStatusWithMessage(ICM_sensor, "setSampleMode returned: ");

    // Set full scale ranges for both acc and gyr
    ICM_20948_fss_t accAndGyroSensitivity;  // This uses a "Full Scale Settings" structure that can contain values for all configurable sensors

    accAndGyroSensitivity.a = gpm16;    // Set accelerometer to +/- 16g
    accAndGyroSensitivity.g = dps2000;  // Set gyroscope +/- 2000dps

    ICM_sensor.setFullScale( (ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), accAndGyroSensitivity );
    // printICMStatusWithMessage(ICM_sensor, "setFullScale returned: ");

    ICM_sensor.startupMagnetometer();
    // printICMStatusWithMessage(ICM_sensor, "startupMagnetometer returned: ");
    
    // This was from Adafruit library - determine where data rate is set in SparkFun library
    float accel_rate = 53.57;  // 1125 Hz / 21 = output data rate (53.57 Hz), which is longest data rate
    float gyro_rate = 100;    // 1100 Hz / 10 = output data rate (100 Hz)
    // Magnetometer is set to 100 Hz output data rate
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
  if(imu_ready) {
    // Collect IMU sensor data
    if(ICM_sensor.dataReady()) {
      ICM_sensor.getAGMT();   // AGMT = accelerometer, gyroscope, magnetometer, temperature. Updates readings
    }
    
    // Print IMU sensor data (if printData is true)
    if(printData[IMU]) {
      printIMUData();
    }

    // Store IMU sensor data to SD card (if saveToSD is true)
    if(saveToSD[IMU]) {
      storeIMUData(fileName);
    }
  }
}


void printICMStatusWithMessage(char* message) {
    if(ICM_sensor.status != ICM_20948_Stat_Ok){
    Serial.print(message);
    Serial.println(ICM_sensor.statusString());
  }
}
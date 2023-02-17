/*
 * SDLib.h
 * RockSat-X 2022
 * 
 * SD definition file.
 */

#ifndef SDLIB_H
#define SDLIB_H

#include <stdio.h>  // For handling C strings
#include <SD.h>
#include "DataConfig.h"

const int SDCardChipSelect = 13;
extern File sensor_file;
extern bool SD_initialized;

// Configure SD card peripheral
bool init_SD();

/*
* Create SD card file with name fileName
* @param numIndex: index in file name where number is/should go in case of duplicates
*/
void createFile(char* fileName, uint8_t numIndex);

// Handle writing the time to the SD card file
void writeTimestamp(char* fileName, float time);

// Handle writing a message to the SD card file
void writeMessage(char* fileName);

#endif  /* SDLIB_H */

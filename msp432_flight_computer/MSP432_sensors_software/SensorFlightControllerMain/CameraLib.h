#ifndef CAMERALIB_H
#define CAMERALIB_H

// Arducam Mini 5MP Plus OV5642
// https://github.com/ArduCAM/Arduino
// https://www.arducam.com/docs/spi-cameras-for-arduino/software/quick-start-guide/
// https://www.arducam.com/knowledge-base/mini-tutorial/
#include "SDLib.h"

void startCameraVideo(int seconds);
void saveVideoToSD();

#endif /* CAMERALIB_H */
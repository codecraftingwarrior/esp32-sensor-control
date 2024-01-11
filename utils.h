#include <Arduino.h>
#include "data_type.h"
#include <TFT_eSPI.h>  // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#ifndef UTILS_H
#define UTILS_H

String env(String name);
void updateTftDisplay(TFT_eSPI, Sensor, Sensor);
#endif

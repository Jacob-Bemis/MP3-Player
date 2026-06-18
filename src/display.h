#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>


void navigationDisplayTask(Album *albumList);
void displayInit(void);


#endif

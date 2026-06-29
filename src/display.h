#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "scanSD.h"


void navigationDisplayTask(Album *albumList);
void displayInit(void);
void home_screen(Album *albumList, int size);
void album_screen(Album *albumList, int size);
void error_message(void);


#endif

#include "display.h"
#include "shared_resources.h"
#include "scanSD.h"
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

/*
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

// CHANGE THESE TO YOUR ACTUAL WIRES:
#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST  14
#define TFT_BL   15   // or tie BL to 3.3V and remove this

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
uint16_t gray = tft.color565(160, 160, 160); // RGB (0–255)
uint16_t orange = tft.color565(255, 165, 0);


void drawThickLine(int x0, int y0, int x1, int y1, int thicnss, uint16_t color)
{ for (int i = 0; i < thicnss; i++) { tft.drawLine(x0, y0+i, x1, y1+i, color);
  }
}



void setup() {
  Serial.begin(115200);
  delay(300);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  // Explicit SPI pin mapping (ESP32-S3 allows this)
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  Serial.println("Init ST7789...");
  tft.init(240, 320);        // common for Waveshare 2" ST7789
  tft.setRotation(1);
  tft.fillScreen(ST77XX_RED);
  delay(400);
  tft.fillScreen(ST77XX_GREEN);
  delay(400);
  tft.fillScreen(gray);
  delay(400);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(5, 8);
  tft.print("Albums");
  drawThickLine(0, 25, 319, 25, 5, orange);
  drawThickLine(0, 188, 319, 188, 5, orange);
  tft.setTextSize(1);
  tft.setCursor(5, 196);
  tft.print("* No Song Playing *");

}

void loop() {}

*/

#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   6
#define TFT_DC   5
#define TFT_RST  4

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
uint16_t gray = tft.color565(160, 160, 160); // RGB (0–255)
uint16_t orange = tft.color565(255, 165, 0);


void drawThickLine(int x0, int y0, int x1, int y1, int thicnss, uint16_t color) {
  for (int i = 0; i < thicnss; i++) {
    tft.drawLine(x0, y0+i, x1, y1+i, color);
  }
}

void displayInit(void){
    //SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
    tft.init(240, 320);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_RED);
    delay(400);
    tft.fillScreen(ST77XX_GREEN);
    delay(400);
    tft.fillScreen(gray);
    delay(400);

}

/*Displays the home screen*/
void home_screen(Album *albumList, int size) {
    int albumPrint = currentAlbumID - (currentAlbumID % 10);
    int printLimit = 9;
    if (((size - albumPrint) + 1) < 10) {
        printLimit = (size % 10) - 1;
    }
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, 8);
    tft.print("Albums");
    drawThickLine(0, 25, 319, 25, 5, orange);
    tft.setTextSize(1.25);
    for (int i = 0; i < printLimit; i++){
        if (i == currentAlbumID){
            tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        } else{
            tft.setTextColor(ST77XX_WHITE);
        }
        tft.setCursor(5, 35 + (i * 10));
        tft.print(albumList[albumPrint].albumName);
        albumPrint++;
    }
    drawThickLine(0, 167, 319, 167, 5, orange);
}
    /*Displays an album's tracklist*/
void album_screen(Album *albumList, int size) {
    int songPrint = currentAlbumID - (currentAlbumID % 10);
    int printLimit = 9;
    if (((size - songPrint) + 1) < 10) {
        printLimit = (size % 10) - 1;
    }
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, 8);
    tft.print(albumList[currentAlbumID].albumName);
    drawThickLine(0, 25, 319, 25, 5, orange);
    tft.setTextSize(1.25);
    Song *trackList = albumList[currentAlbumID].trackList;
    for (int i = 0; i < printLimit; i++) {
        if (i == currentSongID){
            tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
        } else{
            tft.setTextColor(ST77XX_WHITE);
        }
        tft.setCursor(5, 35 + (i * 12));
        tft.print(trackList[songPrint].name);
        songPrint++;
    }
    drawThickLine(0, 167, 319, 167, 5, orange);

}

/*Displays an error message if it is detected that no SD card is inserted */
void error_message(void) {
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, 8);
    tft.setTextSize(3);
    tft.setTextColor(ST77XX_WHITE);
    tft.print("ERROR: SD card   not detected");
}

// pseudo code for now
void navigationDisplayTask(Album *albumList, int size){
    nav_State currentState = STATE_HOME;
    nav_EVENT event;
    while(1){
        if (xQueueReceive(xEventQueue, &event, portMAX_DELAY) == pdTRUE){
            switch(currentState){

                STATE_HOME:
                if (event == EV_SELECT){
                    currentState = STATE_ALBUM;
                } else if (event == EV_ERROR) {
                    currentState = STATE_ERROR;
                }
                home_screen(albumList, size);
                break;

                STATE_ALBUM:
                if (event == EV_BACK){
                    currentState = STATE_HOME;
                } else if (event == EV_ERROR) {
                    currentState = STATE_ERROR;
                }
                album_screen(albumList, size);
                break;

                STATE_ERROR:
                error_message();
                break;
            }
        }
    }
}

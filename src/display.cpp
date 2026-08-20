#include "display.h"
#include "shared_resources.h"
#include "scanSD.h"
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>


#define TFT_MOSI 11
#define TFT_SCLK 12
#define TFT_CS   6
#define TFT_DC   5
#define TFT_RST  4

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
uint16_t gray = tft.color565(160, 160, 160); // RGB (0–255)
uint16_t orange = tft.color565(255, 165, 0);
bool audioInit = false;


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
        printLimit = (size % 10);
    }
    Serial.println(printLimit);
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, 8);
    tft.print("Albums");
    drawThickLine(0, 25, 319, 25, 5, orange);
    tft.setTextSize(1.25);
    for (int i = 0; i < printLimit; i++){
        if (i == (currentAlbumID % 10)){
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
    int songPrint = currentSongID - (currentSongID % 10);
    int printLimit = 9;
    if (((size - songPrint) + 1) < 10) {
        printLimit = (size % 10);
    }
    tft.setTextSize(2);
    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, 8);
    tft.print(albumList[currentAlbumID].albumName);
    drawThickLine(0, 25, 319, 25, 5, orange);
    tft.setTextSize(1.25);
    Song *trackList = albumList[currentAlbumID].trackList;
    for (int i = 0; i < printLimit; i++) {
        if (i == (currentSongID % 10)){
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
/*TODO: Add binary semaphores to preserve currentAlbumID and currentSongID*/

void navigationDisplayTask(void *pvParameters) {
    Album *albumList = (Album *) pvParameters;
    nav_State currentState = STATE_HOME;
    nav_EVENT event;
    home_screen(albumList, albumCount);
    while(1){
        if (xQueueReceive(xNavEventQueue, &event, portMAX_DELAY) == pdTRUE){
            switch(currentState){

                case STATE_HOME:
                tft.fillScreen(gray);
                if (event == EV_SELECT){
                  currentState = STATE_ALBUM;
                  album_screen(albumList, albumList[currentAlbumID].trackCount);
                } else if (event == EV_UP) {
                    if (currentAlbumID != (albumCount -1)){
                      currentAlbumID++;
                    }
                    home_screen(albumList, albumCount);
                } else if (event == EV_DOWN) {
                  if (currentAlbumID != 0) {
                    currentAlbumID--;
                  }
                  home_screen(albumList, albumCount);
                }else if (event == EV_ERROR) {
                    currentState = STATE_ERROR;
                } else if (event == EV_BACK) {
                    home_screen(albumList, albumCount);
                }
                break;

                case STATE_ALBUM:
                tft.fillScreen(gray);
                if (event == EV_BACK){
                  currentState = STATE_HOME;
                  home_screen(albumList, albumCount);
                } else if (event == EV_UP) {
                    if (currentSongID != (albumList[currentAlbumID].trackCount - 1)){
                      currentSongID++;
                    }
                    album_screen(albumList, albumList[currentAlbumID].trackCount);
                } else if (event == EV_DOWN) {
                  if (currentSongID != 0) {
                    currentSongID--;
                  }
                  album_screen(albumList, albumList[currentAlbumID].trackCount);
                  }else if (event == EV_ERROR) {
                    currentState = STATE_ERROR;
                  } else if (event == EV_SELECT) {
                    // put binary semaphores
                    if (!audioInit){
                      xTaskNotifyGive(xAudioPlaybackTask);
                      audioInit = true;
                    }else if (mp3Playing){
                      /*TODO: add a binary semaphore to preserve audio_interupt
                       * and mp3Playing . it is a shared resource*/
                    playback_EVENT ev = EV_STOP;
                    xQueueSend(xPlayEventQueue, &ev, 0);
                    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE){
                    mp3Playing = true;
                    currentAlbumPlaying = currentAlbumID;
                    currentTrackPlaying = currentSongID;
                    xSemaphoreGive(xMutex);
                    }
                    vTaskDelay(pdMS_TO_TICKS(10));
                    }
                  }
                break;

                case STATE_ERROR:
                tft.fillScreen(gray);
                error_message();
                break;
            }
        }
    }
    }

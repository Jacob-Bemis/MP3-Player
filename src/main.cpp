/*
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <scanSD.h>
#include <display.h>
#include "buttons.h"
#include "shared_resources.h"
Album albums[MAX_ALBUM];



void setup() {
    Serial.begin(115200);
//    while(!Serial);
    delay(300);
    xEventQueue = xQueueCreate(5, sizeof(nav_EVENT));
    configASSERT(xEventQueue != NULL);
    SPI.begin(12, 13, 11);
    displayInit();
    buttonInit();
    Serial.println("Parsing");
    parseSD(albums);


    xTaskCreatePinnedToCore(navigationDisplayTask, "Navigation Display Task",
4096, &albums, 1, NULL, 0); xTaskCreatePinnedToCore(buttonTask, "Button Task",
4096, NULL, 2, NULL, 0);

   // album_screen(albums, albums[0].trackCount);
}


void loop() {

}
*/

#include <Arduino.h>
#include "driver/i2s.h"
#include <cstdint>
#include <math.h>
#include "../lib/libhelix-mp3/mp3dec.h"
#include "audio.h"
#include "../assets/test_song_data.h"

#define AMPLITUDE 3000
#define MAX_SAMPLES 250 
#define FREQUENCY 440
#define PI  3.14159265358979323846

#define BCLK_PIN 36
#define LRC_PIN 38
#define DIN_PIN 37
#define I2S_PIN_NO_CHANGE 35

int16_t PCM_buffer[MAX_SAMPLES * 2];
float phase = 0;
int16_t sample = 0;
size_t bytes_written;

int frame = 0;
HMP3Decoder MP3_Decoder;
int counter = 0;

void setup() {
  MP3_Decoder = MP3InitDecoder();
  i2s_init();
}

void loop() {
  frame = MP3FindSyncWord(keyboard_test_mp3, keyboard_test_mp3_len);
  keyboard_test_mp3 += frame;
  counter = keyboard_test_mp3_len - frame; // frame is the offset
  while (counter > 0){
  int decode_flag = MP3Decode(MP3_Decoder, &keyboard_test_mp3, &counter, PCM_buffer, 0);
  if (decode_flag != 0){
      Serial.print("ERROR");
  }
  i2s_write(I2S_NUM_0, PCM_buffer, buffer_size, &bytes_written, portMAX_DELAY);
  }
}

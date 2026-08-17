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
#include <SPI.h>
#include <SD.h>
#include <scanSD.h>
#include "shared_resources.h"
Album albums[MAX_ALBUM];

void setup() {
    Serial.begin(115200);
    delay(2000);
    SPI.begin(12, 13, 11);
    parseSD(albums);
    i2s_init();
    xTaskCreate(
        mp3PlaybackTask,     // function
        "mp3PlaybackTask",   // name
        16384,          // stack size in bytes — generously larger than default
        NULL,           // param
        1,              // priority
        NULL            // task handle (not needed here)
    );
}

void loop() {
}

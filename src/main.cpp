
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


    xTaskCreatePinnedToCore(navigationDisplayTask, "Navigation Display Task",  4096, &albums, 1, NULL, 0);
    xTaskCreatePinnedToCore(buttonTask, "Button Task", 4096, NULL, 2, NULL, 0);

   // album_screen(albums, albums[0].trackCount);
}


void loop() {

}

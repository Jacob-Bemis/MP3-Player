
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <scanSD.h>
#include <display.h>
Album albums[MAX_ALBUM];



void setup() {
    Serial.begin(115200);
   // while (!Serial)
    delay(300);
    SPI.begin(12, 13, 11);
    displayInit();
    Serial.println("Parsing");
    parseSD(albums);


    error_message();
}


void loop(){

}

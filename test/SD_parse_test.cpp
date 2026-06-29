/*
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <scanSD.h>

Album albums[MAX_ALBUM];



void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    Serial.println("Parsing");
    parseSD(albums);

    Serial.println("Albums found:");
    for (int i = 0; i < albumCount; i++) {
        Serial.printf("%d: %s\n", i, albums[i].albumName.c_str());
        Serial.println("Tracklist:");
        for (int j =0; j < albums[i].trackCount; j++){
            Serial.printf("%d: %s\n", j, albums[i].trackList[j].songFilePath.c_str());
        }
    }



}

void loop(){

}
*/

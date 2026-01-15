/*#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <scanSD.h>

Album albums[MAX_ALBUM];



void setup() {
  Serial.begin(115200);
  parseSD(albums);

  Serial.println("Albums found:");
  for (int i = 0; i < albumCount; i++) {
    Serial.printf("%d: %s\n", i, albums[i].albumName.c_str());
    Serial.println("Tracklist:");
    for (int j =0; j < albums[i].trackCount; j++){
      Serial.printf("%d: %s\n", j, albums[i].trackList[j].name.c_str());
    }
  }

 

}

void loop(){

}
*/
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

static const int SD_CS   = 5;
static const int SD_SCK  = 12;
static const int SD_MOSI = 11;
static const int SD_MISO = 4;   // important: avoid 13 for now

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nSD minimal bring-up");

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  Serial.println("SD.begin @ 100kHz...");
  if (!SD.begin(SD_CS, SPI, 100000)) {
    Serial.println("SD.begin FAILED");
    return;
  }

  Serial.println("SD.begin OK");
  Serial.print("Card size (MB): ");
  Serial.println((uint32_t)(SD.cardSize() / (1024 * 1024)));
}

void loop() {}

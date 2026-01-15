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


void drawThickLine(int x0, int y0, int x1, int y1, int thicnss, uint16_t color) {
  for (int i = 0; i < thicnss; i++) {
    tft.drawLine(x0, y0+i, x1, y1+i, color);
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

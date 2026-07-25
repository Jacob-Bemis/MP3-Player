/*
 #include <Arduino.h>
#include "driver/i2s.h"
#include <cstdint>
#include <math.h>
#include "../lib/libhelix-mp3/mp3dec.h"
#include "audio.h"
#include "../assets/test_song_data.h"

int frame = 0;
HMP3Decoder MP3_Decoder;
int counter = 0;
unsigned char *MP3_bytes = (unsigned char *)keyboard_test_mp3;

#define MAX_SAMPLES 1152
int16_t PCM_buffer[MAX_SAMPLES * 2];
size_t bytes_written;
MP3FrameInfo frameInfo;

void decodeTask(void *param) {
    MP3_Decoder = MP3InitDecoder();
    i2s_init();

    MP3_bytes = (unsigned char *)keyboard_test_mp3;
    frame = MP3FindSyncWord(MP3_bytes, keyboard_test_mp3_len);
    if (frame < 0) {
        Serial.println("No sync word found!");
        vTaskDelete(NULL);
    }
    MP3_bytes += frame;
    counter = keyboard_test_mp3_len - frame;

    while (counter > 0) {
        Serial.print("Decoding from: ");
        for (int i = 0; i < 8; i++) Serial.printf("%02X ", MP3_bytes[i]);
        Serial.println();
        int decode_flag = MP3Decode(MP3_Decoder, &MP3_bytes, &counter, PCM_buffer, 0);

        if (decode_flag != 0) {
            Serial.println(decode_flag);
            int resync = MP3FindSyncWord(MP3_bytes, counter);
            if (resync < 0) break;
            MP3_bytes += resync;
            counter -= resync;
            continue;
        }
        MP3GetLastFrameInfo(MP3_Decoder, &frameInfo);
        // for mono MP3 files
        if (frameInfo.nChans == 1) {
          for (int i = frameInfo.outputSamps - 1; i >= 0; i--) {
            PCM_buffer[i * 2] = PCM_buffer[i];
            PCM_buffer[i*2 + 1] = PCM_buffer[i];
          }
        }
        int frameBytes = frameInfo.outputSamps *2 * sizeof(int16_t);
        i2s_write(I2S_NUM_0, PCM_buffer, frameBytes, &bytes_written, portMAX_DELAY);
    }
    Serial.println("Done decoding.");
    vTaskDelete(NULL);
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("1: starting");
    Serial.println("First bytes of MP3 data:");
    for (int i = 0; i < 32; i++) {
        Serial.printf("%02X ", keyboard_test_mp3[i]);
    }
    Serial.println();
    Serial.print("Array length reported: ");
    Serial.println(keyboard_test_mp3_len);

    xTaskCreate(
        decodeTask,     // function
        "decodeTask",   // name
        16384,          // stack size in bytes — generously larger than default
        NULL,           // param
        1,              // priority
        NULL            // task handle (not needed here)
    );
}

void loop() {
}

 */

#include "audio.h"
#include "../lib/libhelix-mp3/mp3dec.h"
#include "driver/i2s.h"
#include <string.h>
#include <Arduino.h>
#include <SD.h>

#define AMPLITUDE 3000
#define FREQUENCY 440
//#define PI  3.14159265358979323846

#define BCLK_PIN 48
#define LRC_PIN 21
#define DIN_PIN 47
//#define I2S_PIN_NO_CHANGE 35

int frame = 0;
HMP3Decoder MP3_Decoder;
int counter = 0;

#define MAX_SAMPLES 1152
int16_t PCM_buffer[MAX_SAMPLES * 2];
size_t bytes_written;
MP3FrameInfo frameInfo;

float phase = 0;
int16_t sample = 0;
#define BUFFER_SIZE 0x00001000
#define REFRESH_THRESHOLD 0x00000A00
unsigned char ringBuffer[BUFFER_SIZE];
bool fileDone = false;
int frameBytes;

void i2s_init(void) {
    i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .dma_buf_count = 8,
    .dma_buf_len = 256,
    };
    i2s_pin_config_t pin_config = {
    .bck_io_num = BCLK_PIN,
    .ws_io_num = LRC_PIN,
    .data_out_num = DIN_PIN,
    .data_in_num = I2S_PIN_NO_CHANGE,
    };
    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);

}

void mp3PlaybackTask(void *param) {
    MP3_Decoder = MP3InitDecoder();
    File mp3File = SD.open("/Music/Swans - Soundtracks for the Blind/1-12-animus.mp3");
    size_t fileSize = mp3File.size();
    size_t fileSizeCount = fileSize;
    int index = 0;
    while ((index + 1) <= BUFFER_SIZE && (index + 1) <= fileSize){
      int mp3_byte = mp3File.read();
      ringBuffer[index] = (unsigned char) mp3_byte;
      index++;
    }
    unsigned char *read_pointer = ringBuffer;
    frame = MP3FindSyncWord(read_pointer, index);
    if (frame < 0) {
        Serial.println("No sync word found!");
        vTaskDelete(NULL);
    }
    read_pointer += frame;
    counter = index - frame;

    while (counter > 0) {
      //fileSizeCount -= (fileSizeCount - counter);
      if (!fileDone && counter <= (BUFFER_SIZE - REFRESH_THRESHOLD) && fileSize > BUFFER_SIZE) {
          memmove(ringBuffer, read_pointer, counter);
          index = counter;
          while ((index + 1) <= BUFFER_SIZE) {
            int mp3_byte = mp3File.read();
            if (mp3_byte == -1) {
                fileDone = true;
                break;
            }
              ringBuffer[index] = (unsigned char) mp3_byte;
              index++;
          }
          read_pointer = ringBuffer;
          counter = index;
      }
        int decode_flag = MP3Decode(MP3_Decoder, &read_pointer, &counter,
PCM_buffer, 0);

        if (decode_flag != 0) {
            Serial.println(decode_flag);
            int resync = MP3FindSyncWord(read_pointer, counter);
            if (resync < 0) break;
            read_pointer += resync;
            counter -= resync;
            continue;
        }
        MP3GetLastFrameInfo(MP3_Decoder, &frameInfo);
        frameBytes = frameInfo.outputSamps * sizeof(int16_t);
        // for mono MP3 files
        if (frameInfo.nChans == 1) {
          for (int i = frameInfo.outputSamps - 1; i >= 0; i--) {
            PCM_buffer[i * 2] = PCM_buffer[i];
            PCM_buffer[i*2 + 1] = PCM_buffer[i];
          }
          frameBytes = frameInfo.outputSamps * 2 * sizeof(int16_t);
        }
        i2s_write(I2S_NUM_0, PCM_buffer, frameBytes, &bytes_written,portMAX_DELAY);
    }
    mp3File.close();
    Serial.println("Done decoding.");
    vTaskDelete(NULL);
}

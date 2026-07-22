/*

  Test for getting a clean tone

#include <Arduino.h>
#include "driver/i2s.h"
#include <cstdint>
#include <math.h>

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

void setup() {
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

void loop() {
    for (int i = 0; i < MAX_SAMPLES; i++){
        phase += (2*PI * FREQUENCY) / 44100;
        if (phase >= 2*PI){
            phase -= 2*PI;
        }
        sample = (int16_t) AMPLITUDE * sin(phase);
        PCM_buffer[i * 2] = sample;
        PCM_buffer[i*2 +1] = sample;
    }
    int buffer_size =  sizeof(PCM_buffer);
    i2s_write(I2S_NUM_0, PCM_buffer, buffer_size, &bytes_written, portMAX_DELAY);
}
 */

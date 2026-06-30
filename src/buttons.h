#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <Arduino.h>

struct Btn {
    int pin;x
    bool lastStable, lastRead;
    uint32_t lastChange, lastFire;
};

typedef enum {SELECT, BACK, UP, DOWN} button_Events;

extern QueueHandle_t xButtonEventQueue;

void buttonTask(void *);
void buttonInit(void);

#endif

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <Arduino.h>

struct Btn {
    int pin;
    bool lastStable, lastRead;
    uint32_t lastChange, lastFire;
};

void buttonTask(void *);
void buttonInit(void);

#endif

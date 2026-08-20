#include "buttons.h"
#include "shared_resources.h"
#include <Arduino.h>

const int SELECT_BUTTON_PIN = 42;
const int BACK_BUTTON_PIN = 41;
const int UP_BUTTON_PIN = 40;
const int DOWN_BUTTON_PIN = 39;

// handles debouncing
bool checkButtonPressed(Btn &b);
const unsigned long DEBOUNCE_DELAY = 20;
const uint32_t FIRE_DELAY = 150;

struct Btn;

Btn btns[] = {
    {SELECT_BUTTON_PIN, HIGH, HIGH, 0, 0},
    {BACK_BUTTON_PIN, HIGH, HIGH, 0, 0},
    {UP_BUTTON_PIN, HIGH, HIGH, 0, 0},
    {DOWN_BUTTON_PIN, HIGH, HIGH, 0, 0},
};

void buttonInit(void ) {
    pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BACK_BUTTON_PIN, INPUT_PULLUP);
    pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
}

bool checkButtonPressed(Btn &b) {
  uint32_t now = millis();
  bool currentRead = digitalRead(b.pin);

  if (currentRead != b.lastRead) {
    b.lastChange = now;
    b.lastRead = currentRead;
  }

  if ((now - b.lastChange) > DEBOUNCE_DELAY) {
    if (currentRead != b.lastStable) {
      b.lastStable = currentRead;


      if (b.lastStable == LOW && (now - b.lastFire) > FIRE_DELAY) {
        b.lastFire = now;
        return true; // button was pressed
      }
    }
  }
  return false;
}


void buttonTask(void *) {
    nav_EVENT ev;
    while (1) {
      for (auto &b : btns) {
        if (checkButtonPressed(b)) {
          switch (b.pin) {

              case SELECT_BUTTON_PIN:
              ev = EV_SELECT;
              xQueueSend(xNavEventQueue, &ev, 0);
              break;

              case BACK_BUTTON_PIN:
              ev = EV_BACK;
              xQueueSend(xNavEventQueue, &ev, 0);
              break;

              case UP_BUTTON_PIN:
              ev = EV_UP;
              xQueueSend(xNavEventQueue, &ev, 0);
              break;

              case DOWN_BUTTON_PIN:
              ev = EV_DOWN;
              xQueueSend(xNavEventQueue, &ev, 0);
              break;
          }
        }
      }
      vTaskDelay(pdMS_TO_TICKS(10));
    }
}

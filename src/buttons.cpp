#include "buttons.h"
#include "shared_resources.h"
#include <Arduino.h>

volatile unsigned long lastFireTime[4] = {0, 0, 0, 0};

const int SELECT_BUTTON_PIN = 42;
const int BACK_BUTTON_PIN = 41;
const int UP_BUTTON_PIN = 40;
const int DOWN_BUTTON_PIN = 39;

QueueHandle_t xButtonEventQueue;

// handles debouncing
bool checkButtonPressed(Btn &b);
// Interrupt handlers
void IRAM_ATTR selectButtonHandler();
void IRAM_ATTR backButtonHandler();
void IRAM_ATTR upButtonHandler();
void IRAM_ATTR downButtonHandler();

const unsigned long DEBOUNCE_DELAY = 20;
const uint32_t FIRE_DELAY = 150;
uint32_t now;

struct Btn;

Btn btns[] = {
    {SELECT_BUTTON_PIN, HIGH, HIGH, 0, 0},
    {BACK_BUTTON_PIN, HIGH, HIGH, 0, 0},
    {UP_BUTTON_PIN, HIGH, HIGH, 0, 0},
    {DOWN_BUTTON_PIN, HIGH, HIGH, 0, 0},
};

void IRAM_ATTR selectButtonHandler() {
  // Serial.println("Select Button Pressed");
    unsigned long now = millis();
    if ((now - lastFireTime[0]) < DEBOUNCE_MS) return;
    lastFireTime[0] = now;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    nav_EVENT ev = EV_SELECT;
    xQueueSendFromISR(xEventQueue, &ev, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void IRAM_ATTR backButtonHandler() {
  // Serial.println("Back Button Pressed");
    unsigned long now = millis();
    if ((now - lastFireTime[1]) < DEBOUNCE_MS) return;
    lastFireTime[1] = now;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    nav_EVENT ev = EV_BACK;
    xQueueSendFromISR(xEventQueue, &ev, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void IRAM_ATTR upButtonHandler() {
  // Serial.println("Up Button Pressed");
    unsigned long now = millis();
    if ((now - lastFireTime[2]) < DEBOUNCE_MS) return;
    lastFireTime[2] = now;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    nav_EVENT ev = EV_UP;
    xQueueSendFromISR(xEventQueue, &ev, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void IRAM_ATTR downButtonHandler() {
  // Serial.println("Down Button Pressed");
    unsigned long now = millis();
    if ((now - lastFireTime[3]) < DEBOUNCE_MS) return;
    lastFireTime[3] = now;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    nav_EVENT ev = EV_DOWN;
    xQueueSendFromISR(xEventQueue, &ev, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void buttonInit(void ) {
    pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BACK_BUTTON_PIN, INPUT_PULLUP);
    pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(SELECT_BUTTON_PIN), selectButtonHandler, FALLING);
    attachInterrupt(digitalPinToInterrupt(BACK_BUTTON_PIN), backButtonHandler, FALLING);
    attachInterrupt(digitalPinToInterrupt(UP_BUTTON_PIN), upButtonHandler, FALLING);
    attachInterrupt(digitalPinToInterrupt(DOWN_BUTTON_PIN), downButtonHandler, FALLING);
}

bool checkButtonPressed(Btn &b) {
  now = millis();
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

/*
void buttonTask(void *) {
    button_Events bEvent;
    nav_EVENT event;
    while (1) {
      if (xQueueReceive(xButtonEventQueue, &bEvent, portMAX_DELAY) == pdTRUE){
          switch(bEvent){

              case SELECT:
                  if (checkButtonPressed(btns[0])){
                      event = EV_SELECT;
                      xQueueSend(xEventQueue, &event, 0);
                  }
              break;

              case BACK:
                  if (checkButtonPressed(btns[1])){
                      event = EV_BACK;
                      xQueueSend(xEventQueue, &event, 0);
                  }
              break;

              case UP:
                  if (checkButtonPressed(btns[2])){
                      event = EV_UP;
                      xQueueSend(xEventQueue, &event, 0);
                  }
              break;

              case DOWN:
                  if (checkButtonPressed(btns[3])){
                      event = EV_DOWN;
                      xQueueSend(xEventQueue, &event, 0);
                  }
              break;
          }
      }
    }
    }

    */

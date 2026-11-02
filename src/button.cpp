#include "button.h"

static uint8_t buttonPin;

static bool lastUnstableState = HIGH;
static bool stableState = HIGH;

static uint32_t debounceTimer;
static uint32_t pressTimer;

static bool longPress = false;

const uint32_t DEBOUNCE_TIME = 25;
const uint32_t LONG_PRESS_TIME = 1500;

void buttonBegin(int8_t pin) {

    buttonPin = pin;
    pinMode(buttonPin, INPUT_PULLUP);

    lastUnstableState = digitalRead(buttonPin);
    stableState = lastUnstableState;
}

ButtonEvent buttonTick() {
    
    bool unstableState = digitalRead(buttonPin);

    if (unstableState != lastUnstableState) {
        debounceTimer = millis();
        lastUnstableState = unstableState;
    }

    if (millis() - debounceTimer >= DEBOUNCE_TIME) {

        if (lastUnstableState != stableState) {

            stableState = unstableState;

            if (stableState == LOW) {

                pressTimer = millis();
                longPress = false;
            }
            else {

                if (!longPress) {
                    return BUTTON_SHORT_PRESS;
                }
            }
        }
        if (stableState == LOW && !longPress && millis() - pressTimer >= LONG_PRESS_TIME) {

            longPress = true;
            return BUTTON_LONG_PRESS;
        }
    }
    return BUTTON_NONE;
}
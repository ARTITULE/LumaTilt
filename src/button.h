#pragma once
#include <Arduino.h>

enum ButtonEvent {
    BUTTON_NONE,
    BUTTON_SHORT_PRESS,
    BUTTON_LONG_PRESS,
};

void buttonBegin(int8_t pin);
ButtonEvent buttonTick();
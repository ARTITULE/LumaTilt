#include <Arduino.h>

#include "display.h"
#include "button.h"
#include "config.h"

#define BUTTON_PIN 0

void flashingMode();
void walkingPixelMode();

void setup() {

    displayBegin();
    buttonBegin(BUTTON_PIN);

}

void loop() {

    flashingMode();
    walkingPixelMode();

}

void flashingMode() {

    while (1) {

        ButtonEvent event = buttonTick();
        if (event == BUTTON_SHORT_PRESS) {
            return;
        }

        static bool state;
        static uint32_t flashTimer = millis();
        if (millis() - flashTimer >= FLASHING_MODE_FLASH_INTERVAL) {
            flashTimer = millis();
            state = !state;
            fillFramebuffer(state);
        }

        scanDisplay();
    }
}

void walkingPixelMode() {

    while (1) {

        ButtonEvent event = buttonTick();
        if (event == BUTTON_SHORT_PRESS) {
            return;
        }

        static uint8_t x;
        static uint8_t y;
        static uint32_t pixelTimer = millis();
        if (millis() - pixelTimer >= WALKING_PIXEL_MODE_INTERVAL) {
            pixelTimer = millis();

            clearFramebuffer();
            
            setPixel(x, y, true);

            x++;

            if (x >= MATRIX_WIDTH) {
                x = 0;
                y++;
                if (y >= MATRIX_HEIGHT) {
                    y = 0;
                }
            }
        
        }
        scanDisplay();
    }
}
#include <Arduino.h>

#include "display.h"
#include "config.h"

void flashingMode();
void walkingPixelMode();

void setup() {

    displayBegin();

}

void loop() {

    flashingMode();
    walkingPixelMode();

}

void flashingMode() {

    while (1) {

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
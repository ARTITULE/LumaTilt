#include <Arduino.h>
#include <SPI.h>
#include "display.h"
#include "config.h"

uint8_t frameBuffer[MATRIX_HEIGHT][MATRIX_WIDTH / 8];

void displayBegin() {

    digitalWrite(ROW_OE, LOW);
    digitalWrite(ROW_LATCH, LOW);
    digitalWrite(COL_LATCH, LOW);
    digitalWrite(COL_BLANK, HIGH);

    pinMode(ROW_OE, OUTPUT);
    pinMode(ROW_LATCH, OUTPUT);

    pinMode(COL_LATCH, OUTPUT);
    pinMode(COL_BLANK, OUTPUT);
    
    clearFramebuffer();

    SPI.begin(SPI_SCLK, -1, SPI_MOSI, -1);

}

void clearFramebuffer() {

    for (uint8_t row = 0; row < MATRIX_HEIGHT; row++) {
        for (uint8_t column = 0; column < MATRIX_WIDTH / 8; column++) {

            frameBuffer[row][column] = 0;

        }
    }
    
}

void setPixel(uint8_t x, uint8_t y, bool state) {

    if (x < MATRIX_WIDTH && y < MATRIX_HEIGHT) {
        uint8_t byteIndex = x / 8;
        uint8_t bitInByteIndex = x % 8;
        
        if (state) {
            frameBuffer[y][byteIndex] |= 1 << bitInByteIndex;
        }
        else {
            frameBuffer[y][byteIndex] &= ~(1 << bitInByteIndex);
        }
        
    }
}

void sendSPIByte(uint8_t data) {

    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));

    SPI.transfer(data);

    SPI.endTransaction();
}

void latchRows() {

    digitalWrite(ROW_LATCH, LOW);
    digitalWrite(ROW_LATCH, HIGH);
    digitalWrite(ROW_LATCH, LOW);
}

void latchColumns() {

    digitalWrite(COL_LATCH, LOW);
    digitalWrite(COL_LATCH, HIGH);
    digitalWrite(COL_LATCH, LOW);
}

void setRowsEnabled(bool enabled) {

    digitalWrite(ROW_OE, enabled);
}

void setColumnsEnabled(bool enabled) {

    digitalWrite(COL_BLANK, !enabled);
}

void writeRows(uint32_t data) {

    setRowsEnabled(false);

    uint8_t firstRow = data >> 16;
    sendSPIByte(firstRow);
    uint8_t secondRow = data >> 8;
    sendSPIByte(secondRow);
    uint8_t thirdRow = data;
    sendSPIByte(thirdRow);
    
    latchRows();
}

void selectRow(uint8_t row) {

    if (row < 24) {

        uint32_t rowData = 1;
        rowData = rowData << row;
        rowData = ~rowData;
        writeRows(rowData);
    }
}
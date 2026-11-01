#include <Arduino.h>
#include <SPI.h>
#include "display.h"
#include "config.h"

#define SCAN_INTERVAL_US 208

uint8_t frameBuffer[MATRIX_HEIGHT][MATRIX_WIDTH / 8];
uint32_t lastScanTime;
uint8_t currentRow;


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

void fillFramebuffer(bool state) {

    for (uint8_t row = 0; row < MATRIX_HEIGHT; row++) {
        for (uint8_t column = 0; column < MATRIX_WIDTH / 8; column++) {

            frameBuffer[row][column] = (state ? 0xFF : 0x00);
        }
    }
}

void scanDisplay() {

    if (micros() - lastScanTime >= SCAN_INTERVAL_US) {
        lastScanTime = micros();

        scanRow(currentRow);
        currentRow += (currentRow < MATRIX_HEIGHT - 1 ? 1 : - MATRIX_HEIGHT + 1);
    }

}

void sendSPIData(uint8_t* data, size_t length) {

    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    SPI.transfer(data, length);
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

void writeRows(uint8_t data[3]) {

    setRowsEnabled(false);
    sendSPIData(data, 3);
    latchRows();
}

void selectRow(uint8_t row) {

    if (row < 24) {

        uint32_t rowData = 1;
        rowData = rowData << row;
        rowData = ~rowData;
        uint8_t sentData[3];
        sentData[0] = rowData >> 16;
        sentData[1] = rowData >> 8;
        sentData[2] = rowData;
        writeRows(sentData);
    }
}

void writeColumns(uint8_t data[10]) {

    setColumnsEnabled(false);
    sendSPIData(data, 10);
    latchColumns();

}

void prepareColumnData(uint8_t row, uint8_t data[10]) {

    if (row >= MATRIX_HEIGHT) {
        return;
    }
    for (uint8_t i = 0; i < MATRIX_WIDTH / 8; i++) {

        data[i] = frameBuffer[row][i];
    }
    data[MATRIX_WIDTH / 8] = 0;
}

void scanRow(uint8_t row) {
    
    if (row >= MATRIX_HEIGHT) {
        return;
    }
    setRowsEnabled(false);
    setColumnsEnabled(false);

    uint8_t data[10];

    prepareColumnData(row, data);
    writeColumns(data);
    selectRow(row);

    setRowsEnabled(true);
    setColumnsEnabled(true);
}


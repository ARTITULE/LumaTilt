#pragma once

extern void displayBegin();
extern void clearFramebuffer();
extern void setPixel(uint8_t x, uint8_t y, bool state);
extern void fillFramebuffer(bool state);
extern void scanDisplay();

extern void sendSPIData(uint8_t* data, size_t length);
extern void latchRows();
extern void latchColumns();
extern void setRowsEnabled(bool enabled);
extern void setColumnsEnabled(bool enabled);

extern void writeRows(uint32_t data);
extern void selectRow(uint8_t row);
extern void writeColumns(uint8_t data[10]);
extern void prepareColumnData(uint8_t row, uint8_t data[10]);
extern void scanRow(uint8_t row);

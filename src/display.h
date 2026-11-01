#pragma once

extern void displayBegin();
extern void clearFramebuffer();
extern void setPixel(uint8_t x, uint8_t y, bool state);

extern void sendSPIByte(uint8_t data);
extern void latchRows();
extern void latchColumns();
extern void setRowsEnabled(bool enabled);
extern void setColumnsEnabled(bool enabled);

extern void writeRows(uint32_t data);
extern void selectRow(uint8_t row);

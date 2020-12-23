void displaySpiBegin(void);
void displayBegin(void);
void displayOn(bool on);
void writeData(uint8_t c);
void writeCommand(uint8_t c);
uint8_t readData(void);
void writeReg(const uint8_t reg, uint8_t val);
void waitBusy(uint8_t res);
void textMode(void);
void textWrite(const char *buffer);
void textEnlarge(uint8_t scale);
void setTextCursor(uint16_t x, uint16_t y);
void setTextColor(uint16_t foreColor, uint16_t bgColor);
void drawPixel(int16_t x, int16_t y, uint16_t color);

void clearMemory(bool stop);
void clearActiveWindow(bool full);

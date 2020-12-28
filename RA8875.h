

typedef struct {
  uint16_t	bitmapOffset;
  uint8_t	width;
  uint8_t	height;
  uint8_t	xAdvance;
  int8_t	xOffset;
  int8_t	yOffset;
} GFXglyph;

typedef struct {
  uint8_t	*bitmap;
  GFXglyph	*glyph;
  uint8_t	first;
  uint8_t	last;
  uint8_t	yAdvance;
} GFXfont;

class raspiRA8875 {

public:

raspiRA8875(uint8_t cs, uint8_t rst);
unsigned long millis(void);
void displaySpiBegin(void);
void displayBegin(void);
void displayOn(bool on);
void writeData(uint8_t c);
void writeCommand(uint8_t c);
uint8_t readData(void);
void writeReg(const uint8_t reg, uint8_t val);
void waitBusy(uint8_t res);
void textMode(void);
void graphicsMode(void);
void textWrite(const char *buffer);
void textEnlarge(uint8_t scale);
void setTextCursor(uint16_t x, uint16_t y);
void setTextColor(uint16_t foreColor, uint16_t bgColor);
void drawPixel(int16_t x, int16_t y, uint16_t color);

void clearMemory(bool stop);
void clearActiveWindow(bool full);

void setFontGFX(const GFXfont *f = NULL);
void drawCharGFX(int16_t x, int16_t y, const char *c, uint16_t color, uint8_t size);

private:
  uint8_t  _scale = 0;
  bool     _textMode = false;
  uint8_t  _cs, _rst;

  GFXfont  *gfxFont;
};

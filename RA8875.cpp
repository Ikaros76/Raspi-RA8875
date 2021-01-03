#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <bcm2835.h>

#include "RA8875.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))

inline GFXglyph *pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c){
  return gfxFont->glyph+c;
}

inline uint8_t *pgm_read_bitmap_ptr(const GFXfont *gfxFont) {
  return gfxFont->bitmap;
}

raspiRA8875::raspiRA8875(uint8_t cs, uint8_t rst) {
  _cs = cs;
  _rst = rst;
}

unsigned long raspiRA8875::millis(void) //unsigned long
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ( ts.tv_sec * 1000 + ts.tv_nsec / 1000000L );
}

void raspiRA8875::SPIBegin(void) {
  bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
}

void raspiRA8875::writeData(uint8_t c) {
  bcm2835_gpio_write(_cs, LOW);
  bcm2835_spi_transfer(0x00);
  bcm2835_spi_transfer(c);
  bcm2835_gpio_write(_cs, HIGH);
}

void raspiRA8875::writeCommand(uint8_t c) {
  bcm2835_gpio_write(_cs, LOW);
  bcm2835_spi_transfer(0x80);
  bcm2835_spi_transfer(c);
  bcm2835_gpio_write(_cs, HIGH);
}

uint8_t raspiRA8875::readData(void) {
  bcm2835_gpio_write(_cs, LOW);
  bcm2835_spi_transfer(RA8875_DATAREAD);
  uint8_t c = bcm2835_spi_transfer(0x0);
  bcm2835_gpio_write(_cs, HIGH);
  return c;
}

void raspiRA8875::writeReg(const uint8_t reg, uint8_t val) {
  writeCommand(reg);
  writeData(val);
}

void raspiRA8875::waitBusy(uint8_t res) {
  uint8_t temp;
  unsigned long start = millis();
  do {
  	if (res == 0x01) writeCommand(RA8875_DMACR);
  	bcm2835_gpio_write(_cs, LOW);
  	bcm2835_spi_transfer(RA8875_CMDREAD);
  	temp = bcm2835_spi_transfer(0x0);
  	bcm2835_gpio_write(_cs, HIGH);
  	if ((millis() - start) > 10) return;
  } while ((temp & res) == res);
}

bool raspiRA8875::displayBegin(enum RA8875sizes size) {
  _size = size;
  uint8_t pixclk;
  uint8_t hsync_start;
  uint8_t hsync_pw;
  uint8_t hsync_finetune;
  uint8_t hsync_nondispl;
  uint8_t vsync_pw;
  uint16_t vsync_nondisp;
  uint16_t vsync_start;
  if (_size == RA8875_480x80) {
    pixclk = RA8875_PCSR_PDATL | RA8875_PCSR_4CLK;
    hsync_nondispl = 10;
    hsync_start = 8;
    hsync_pw = 48;
    hsync_finetune = 0;
    vsync_nondisp = 3;
    vsync_start = 8;
    vsync_pw = 10;
    _voffset = 192;
    _width = 480;
    _height = 80;
  } else if (_size == RA8875_480x128) {
    _width = 480;
    _height = 128;
  } else if (_size == RA8875_480x272) {
    _width = 480;
    _height = 272;
  } else if (_size == RA8875_800x480) {
    _width = 800;
    _height = 480;
  } else return false;
  if(bcm2835_init() == -1) {
    printf("BCM2835 init error!\n");
    return false;
  }
  SPIBegin();
  bcm2835_gpio_fsel(_cs, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(_rst, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_write(_cs, HIGH);
  bcm2835_gpio_write(_rst, LOW);
  delay(100);
  bcm2835_gpio_write(_rst, HIGH);
  delay(100);;
  writeReg(RA8875_P1CR,(RA8875_P1CR_ENABLE | (RA8875_PWM_CLK_DIV1024 & 0xF)));
  writeReg(RA8875_P1DCR,255);
  writeReg(RA8875_PLLC1, RA8875_PLLC1_PLLDIV1 + 11);
  delay(1);
  writeReg(RA8875_PLLC2, RA8875_PLLC2_DIV4);
  delay(1);
  writeReg(RA8875_SYSR, RA8875_SYSR_16BPP | RA8875_SYSR_MCU8);
  writeReg(RA8875_PCSR, RA8875_PCSR_PDATL | RA8875_PCSR_4CLK);
  delay(1);
  writeReg(RA8875_HDWR, (800 / 8) - 1);
  writeReg(RA8875_HNDFTR, RA8875_HNDFTR_DE_HIGH + 0);
  writeReg(RA8875_HNDR, (26 - 0 - 2)/8);
  writeReg(RA8875_HSTR, 23 / 8 -1);
  writeReg(RA8875_HPWR, RA8875_HPWR_LOW + (2 / 8 - 1));
  writeReg(RA8875_VDHR0, (uint16_t)(480 -1 + 0) & 0xFF);
  writeReg(RA8875_VDHR1, (uint16_t)(480 -1 + 0) >> 8);
  writeReg(RA8875_VNDR0, 26 - 1);
  writeReg(RA8875_VNDR1, 26 >> 8);
  writeReg(RA8875_VSTR0, 32 - 1);
  writeReg(RA8875_VSTR1, 32 >> 8);
  writeReg(RA8875_VPWR, RA8875_VPWR_LOW + 2 - 1);
  writeReg(RA8875_HSAW0, 0);
  writeReg(RA8875_HSAW1, 0);
  writeReg(RA8875_HEAW0, (uint16_t)(800 - 1) & 0xFF);
  writeReg(RA8875_HEAW1, (uint16_t)(800 - 1) >> 8);
  writeReg(RA8875_VSAW0, 0 + 0);
  writeReg(RA8875_VSAW1, 0 + 0);
  writeReg(RA8875_VEAW0, (uint16_t)(480 - 1 + 0) & 0xFF);
  writeReg(RA8875_VEAW1, (uint16_t)(480 - 1 + 0) >> 8);
  // Clear the entire window
  writeReg(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
  delay(500);
  return true;
}

void raspiRA8875::displayOn(bool on) {
  if (on) writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);
  else writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPOFF);
}

void raspiRA8875::textMode(void) {
  _textMode = true;
  writeCommand(RA8875_MWCR0);
  uint8_t temp = readData();
  temp |= RA8875_MWCR0_TXTMODE;
  writeData(temp);
  writeCommand(0x21);
  temp = readData();
  temp &= ~((1 << 7) | (1 << 5));
  writeData(temp);
}

void raspiRA8875::graphicsMode(void) {
  writeCommand(RA8875_MWCR0);
  uint8_t temp = readData();
  temp &= ~RA8875_MWCR0_TXTMODE;
  writeData(temp);
}

void raspiRA8875::textWrite(const char *buffer) {
  uint16_t len = strlen(buffer);
  for (uint16_t i = 0; i < len; i++) {
  	textEnlarge(_scale);
  	writeCommand(RA8875_MRWC);
  	writeData(buffer[i]);
  	waitBusy(0x80);
  }
}

void raspiRA8875::setTextCursor(uint16_t x, uint16_t y) {
writeCommand(0x2A);
writeData(x & 0xFF);
writeCommand(0x2B);
writeData(x >> 8);
writeCommand(0x2C);
writeData(y & 0xFF);
writeCommand(0x2D);
writeData(y >> 8);
}

void raspiRA8875::textEnlarge(uint8_t scale) {
if (scale > 3)
  scale = 3;
writeCommand(0x22);
uint8_t temp = readData();
temp &= ~(0xF);
temp |= scale << 2;
temp |= scale;
writeCommand(0x22);
writeData(temp);
_scale = scale;
}

void raspiRA8875::setTextColor(uint16_t foreColor, uint16_t bgColor)
{
// set Color Text
writeCommand(0x63);
writeData((foreColor & 0xF800) >> 11);
writeCommand(0x64);
writeData((foreColor & 0x07E0) >> 5);
writeCommand(0x65);
writeData((foreColor & 0x001F));

// set Color Background
writeCommand(0x60);
writeData((bgColor & 0xF800) >> 11);
writeCommand(0x61);
writeData((bgColor & 0x07E0) >> 5);
writeCommand(0x62);
writeData((bgColor & 0x001F));

// clear Transparency flag
writeCommand(0x22);
uint8_t temp = readData();
temp &= ~(1 << 6);
writeData(temp);
}

void raspiRA8875::drawPixel(int16_t x, int16_t y, uint16_t color) {
writeReg(RA8875_CURH0, x);
writeReg(RA8875_CURH1, x >> 8);
writeReg(RA8875_CURV0, y);
writeReg(RA8875_CURV1, y >> 8);
writeCommand(RA8875_MRWC);
bcm2835_gpio_write(_cs, LOW);
bcm2835_spi_transfer(RA8875_DATAWRITE);
bcm2835_spi_transfer(color >> 8);
bcm2835_spi_transfer(color);
bcm2835_gpio_write(_cs, HIGH);
}

void raspiRA8875::clearMemory(bool stop) {
uint8_t temp;
writeCommand(RA8875_MCLR);
if (stop) temp &= ~(1 << 7);
  else temp |= (1 << 7);
writeData(temp);
waitBusy(0x80);
}

void raspiRA8875::clearActiveWindow(bool full) {
uint8_t temp;
writeCommand(RA8875_MCLR);
temp = readData();
if (full == true) temp &= ~(1 << 6);
  else temp |= (1 << 6);
writeData(temp);
}

void raspiRA8875::setFontGFX(const GFXfont *f) {
gfxFont = (GFXfont *)f;
}

void raspiRA8875::drawCharGFX(int16_t x, int16_t y, const char *c, uint16_t color, uint8_t size) {
for(uint16_t t=0;t<= strlen(c)-1; t++) {
  uint8_t z = c[t] - (uint8_t)pgm_read_byte(&gfxFont->first);
  GFXglyph *glyph =  pgm_read_glyph_ptr(gfxFont,z);
  uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);
  uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
  uint8_t  w  = pgm_read_byte(&glyph->width);
  uint8_t  h  = pgm_read_byte(&glyph->height);
  int8_t   xo = pgm_read_byte(&glyph->xOffset);
  int8_t   yo = pgm_read_byte(&glyph->yOffset);
  uint8_t  av = pgm_read_byte(&glyph->xAdvance);
  uint16_t xx, yy, bits = 0,bit = 0;
  int16_t  xo16 = 0, yo16 = 0;
	for(yy=0; yy<h; yy++) {
	for(xx=0; xx<w; xx++) {
	  if(!(bit++ & 7)) {
	    bits = pgm_read_byte(&bitmap[bo++]);
	    }
	  if(bits & 0x80) {
	    if(size==1) drawPixel(x+xo+xx, y+yo+yy, color);
	    }
          bits <<= 1;
       }
     }
     x=x+(av*size);
}
}

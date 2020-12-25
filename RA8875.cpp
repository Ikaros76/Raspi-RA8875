#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <bcm2835.h>

#include "RA8875Register.h"
#include "RA8875.h"

#define RES	2
#define CS	3

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

uint8_t _scale = 0;
bool 	_textMode = false;

raspiRA8875::raspiRA8875(void) {
}

unsigned long raspiRA8875::millis(void) //unsigned long
{
struct timespec ts;
clock_gettime(CLOCK_MONOTONIC, &ts);
return ( ts.tv_sec * 1000 + ts.tv_nsec / 1000000L );
}

void raspiRA8875::displaySpiBegin(void) {
if (bcm2835_init() == -1) {
	printf("SPI init error\n");
	return;
	}
bcm2835_spi_begin();
bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64);
bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
bcm2835_gpio_fsel(CS,BCM2835_GPIO_FSEL_OUTP);
bcm2835_gpio_fsel(RES,BCM2835_GPIO_FSEL_OUTP);
}

void raspiRA8875::writeData(uint8_t c) {
bcm2835_gpio_write(CS, LOW);
bcm2835_spi_transfer(0x00);
bcm2835_spi_transfer(c);
bcm2835_gpio_write(CS, HIGH);
}

void raspiRA8875::writeCommand(uint8_t c) {
bcm2835_gpio_write(CS, LOW);
bcm2835_spi_transfer(0x80);
bcm2835_spi_transfer(c);
bcm2835_gpio_write(CS, HIGH);
}

uint8_t raspiRA8875::readData(void) {
bcm2835_gpio_write(CS, LOW);
bcm2835_spi_transfer(RA8875_DATAREAD);
uint8_t c = bcm2835_spi_transfer(0x0);
bcm2835_gpio_write(CS, HIGH);
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
  bcm2835_gpio_write(CS, LOW);
  bcm2835_spi_transfer(RA8875_CMDREAD);
  temp = bcm2835_spi_transfer(0x0);
  bcm2835_gpio_write(CS, HIGH);
  if ((millis() - start) > 10) return;
} while ((temp & res) == res);
}

void raspiRA8875::textWrite(const char *buffer) {
uint16_t len = strlen(buffer);
//writeCommand(RA8875_MRWC);
for (uint16_t i = 0; i < len; i++) {
textEnlarge(_scale);
writeCommand(RA8875_MRWC);
writeData(buffer[i]);
waitBusy(0x80);
}
}

void raspiRA8875::displayBegin(void) {
bcm2835_gpio_fsel(CS, BCM2835_GPIO_FSEL_OUTP);
bcm2835_gpio_fsel(RES, BCM2835_GPIO_FSEL_OUTP);
bcm2835_gpio_write(CS, HIGH);
bcm2835_gpio_write(RES, LOW);
delay(100);
bcm2835_gpio_write(RES, HIGH);
delay(100);;
//writeReg(0x01, 0x00 | 0x80);
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

void raspiRA8875::displayOn(bool on) {
if (on)
  writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);
else
  writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPOFF);
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
bcm2835_gpio_write(CS, LOW);
bcm2835_spi_transfer(RA8875_DATAWRITE);
bcm2835_spi_transfer(color >> 8);
bcm2835_spi_transfer(color);
bcm2835_gpio_write(CS, HIGH);
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

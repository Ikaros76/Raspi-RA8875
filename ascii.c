#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "RA8875.h"
#include "RA8875Register.h"
#include "Fonts/FreeSans24pt7b.h"

#include <bcm2835.h>

raspiRA8875 tft = raspiRA8875();

int main(void)
{
tft.setFontGFX(&FreeSans24pt7b);
tft.displaySpiBegin();
tft.displayBegin();
tft.displayOn(true);
for (;;){
uint16_t x = (rand() % 800)+1;
uint16_t y = (rand() % 480)+1;
uint16_t c = (rand() % 0xFFFF)+1;
tft.drawCharGFX(x,y,"RA8875",c,1);
}
delay(4000);
tft.textMode();
tft.setTextColor(RA8875_WHITE,RA8875_BLACK);
for(;;) {
tft.textEnlarge(0);
tft.setTextCursor(0,0);
for(int c = 0;c<256; c++){
tft.writeCommand(RA8875_MRWC);
tft.writeData(c);
}
delay(1000);
tft.clearMemory(false);
tft.setTextCursor(0,0);
tft.textEnlarge(1);
for(int c = 0; c < 256; c++) {
tft.writeCommand(RA8875_MRWC);
tft.writeData(c);
}
delay(1000);
tft.clearMemory(false);
tft.setTextCursor(0,0);
tft.textEnlarge(2);
for(int c = 0; c < 256; c++) {
tft.writeCommand(RA8875_MRWC);
tft.writeData(c);
tft.waitBusy(0x80);
}
delay(1000);
tft.clearMemory(false);
}

}

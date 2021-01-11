#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h>
#include "RA8875.h"
#include <string>
#include <cstring>

#include "Fonts/FreeSans24pt7b.h"
#include "Fonts/comic60pt7b.h"
#include "Fonts/FreeMonoOblique24pt7b.h"
#include "Fonts/FreeSerifBold60pt7b.h"

raspiRA8875 tft = raspiRA8875(3, 2);

int main(void) {
  if(!tft.displayBegin(RA8875_800x480)) {
    printf("LCD not found!\n");
    return false;
  }
  tft.displayOn(true);
  tft.setFontGFX(&FreeSans24pt7b);
  tft.drawCharGFX(0,40,"RA8875 Raspberry PI",RA8875_GREEN,1);
  tft.setFontGFX(&comic60pt7b);
  tft.drawCharGFX(0,160,"Demo RA8875",RA8875_MAGENTA,1);
  delay(3000);
  tft.fillScreen(RA8875_RED);
  delay(500);
  tft.fillScreen(RA8875_GREEN);
  delay(500);
  tft.fillScreen(RA8875_BLUE);
  delay(500);
  tft.fillScreen(RA8875_BLACK);
  tft.setFontGFX(&FreeMonoOblique24pt7b);
  tft.drawCharGFX(350,250,"Line demo",RA8875_WHITE,1);
  delay(2000);
  for (uint16_t t=0;t<=10000;t++) {
    int x0 = (rand() % 800)+1;
    int x1 = (rand() % 800)+1;
    int y0 = (rand() % 480)+1;
    int y1 = (rand() % 480)+1;
    int c = (rand() % 0xFFFF)+1;
    tft.drawLine(x0,y0,x1,y1,c);
  }
  delay(1000);
  tft.fillScreen(RA8875_BLACK);
  tft.drawCharGFX(350,250,"Rect demo",RA8875_WHITE,1);
}

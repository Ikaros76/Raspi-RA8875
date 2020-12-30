#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "RA8875.h"

#include "Fonts/FreeSans24pt7b.h"
#include "Fonts/comic60pt7b.h"
#include "Fonts/FreeMonoOblique24pt7b.h"
#include "Fonts/FreeSerifBold60pt7b.h"

raspiRA8875 tft = raspiRA8875(3,2);

int main(void) {
  if(!tft.displayBegin(RA8875_800x480)) {
    printf("LCD not found!\n");
    while(1);
  }
  tft.displayOn(true);
  tft.setFontGFX(&FreeSans24pt7b);
  tft.drawCharGFX(0,40,"RA8875 Raspberry PI",RA8875_GREEN,1);
  tft.setFontGFX(&comic60pt7b);
  tft.drawCharGFX(0,160,"Raspberry PI",RA8875_MAGENTA,1);
  tft.setFontGFX(&FreeMonoOblique24pt7b);
  tft.drawCharGFX(0,250,"800x480 RA8875",RA8875_BLUE,1);
  tft.setFontGFX(&FreeSerifBold60pt7b);
  tft.drawCharGFX(0,400,"Display 7''",RA8875_RED,1);
}

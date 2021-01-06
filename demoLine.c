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

raspiRA8875 tft = raspiRA8875(3, 2);

int main(void) {
  if(!tft.displayBegin(RA8875_800x480)) {
    printf("LCD not found!\n");
    return false;
  }
  tft.displayOn(true);
  tft.setFontGFX(&FreeSans24pt7b);
  tft.fillScreen(RA8875_RED);
  delay(500);
  tft.fillScreen(RA8875_GREEN);
  delay(500);
  tft.fillScreen(RA8875_BLUE);
  delay(500);
  uint32_t temps = tft.millis();
  for (uint16_t t=0;t<=10000;t++) {
  int x0 = (rand() % 800)+1;
  int x1 = (rand() % 800)+1;
  int y0 = (rand() % 480)+1;
  int y1 = (rand() % 480)+1;
  int c = (rand() % 0xFFFF)+1;
  tft.drawLine(x0,y0,x1,y1,c);
  }
  string pi = "Pi is " + to_string((tft.millis()-temps)/1000);
  int n = pi.length();
  char char_array[n+1];
  strcpy(char_array, pi.c_str());
  tft.drawCharGFX(100,100,char_array,RA8875_WHITE,1);

}

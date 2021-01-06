#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "RA8875.h"

raspiRA8875 tft = raspiRA8875(3, 2);

int main(void) {
  if(!tft.displayBegin(RA8875_800x480)) {
    printf("LCD not found!\n");
    return false;
  }
  tft.displayOn(true);
  for (;;) {
  int x0 = (rand() % 800)+1;
  int x1 = (rand() % 800)+1;
  int y0 = (rand() % 480)+1;
  int y1 = (rand() % 480)+1;
  int c = (rand() % 0xFFFF)+1;
  tft.drawLine(x0,y0,x1,y1,c);
  }
}

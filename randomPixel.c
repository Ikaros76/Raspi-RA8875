#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "RA8875.h"
#include "RA8875Register.h"

raspiRA8875 tft = raspiRA8875(3, 2);

int main(void) {
  if(!tft.displayBegin(RA8875_800x480)) {
    printf("LCD not found!\n");
    return false;
  }
  tft.displayOn(true);
  for (;;) {
  int x = (rand() % 800)+1;
  int y = (rand() % 480)+1;
  int c = (rand() % 0xFFFF)+1;
  tft.drawPixel(x,y,c);
  }
}

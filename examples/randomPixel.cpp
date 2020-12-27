#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "RA8875.h"
#include "RA8875Register.h"

raspiRA8875 tft = raspiRA8875();

int main(void) {
tft.displaySpiBegin();
tft.displayBegin();
tft.displayOn(true);
for (;;) {
int x = (rand() % 800)+1;
int y = (rand() % 480)+1;
int c = (rand() % 0xFFFF)+1;
tft.drawPixel(x,y,c);
}
}

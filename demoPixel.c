#include <stdbool.h>
#include <stdint.h>

#include "RA8875.h"
#include "RA8875Register.h"

int main(void) {
displaySpiBegin();
displayBegin();
displayOn(true);
for (uint16_t x=0; x<=800; x++) {
for (uint16_t y=0; y<=480; y++) {
drawPixel(x,y,0xFFFF);
}
}
}

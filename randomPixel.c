#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "RA8875.h"
#include "RA8875Register.h"

int main(void) {
displaySpiBegin();
displayBegin();
displayOn(true);
for (;;) {
int x = (rand() % 800)+1;
int y = (rand() % 480)+1;
int c = (rand() % 0xFFFF)+1;
drawPixel(x,y,c);
}
}

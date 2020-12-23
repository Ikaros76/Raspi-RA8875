#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "RA8875.h"
#include "RA8875Register.h"

int main(void)
{
displaySpiBegin();
displayBegin();
displayOn(true);
textMode();
setTextColor(RA8875_WHITE,RA8875_BLACK);
setTextCursor(0,0);
for(int c = 0;c<256; c++){
writeCommand(RA8875_MRWC);
writeData(c);
}
textEnlarge(1);
for(int c = 0; c < 256; c++) {
writeCommand(RA8875_MRWC);
writeData(c);
}

}

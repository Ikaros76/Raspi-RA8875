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
textEnlarge(3);
for(;;){
int x = (rand() % 800)+1;
int y = (rand() % 480)+1;
int c = (rand() % 255)+1;
int color = (rand() % 0xFFFF)+1;
setTextCursor(x,y);
setTextColor(0x0000, color);
textEnlarge(3);
writeCommand(RA8875_MRWC);
writeData(c);
}}

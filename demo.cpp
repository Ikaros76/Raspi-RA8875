#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "RA8875.h"
#include "RA8875Register.h"

raspiRA8875 tft = raspiRA8875();
int main(void)
{
tft.displaySpiBegin();
tft.displayBegin();
tft.displayOn(true);
tft.textMode();
for(;;){
int x = (rand() % 800)+1;
int y = (rand() % 480)+1;
int c = (rand() % 255)+1;
int color = (rand() % 0xFFFF)+1;
tft.setTextCursor(x,y);
tft.setTextColor(0x0000, color);
tft.writeCommand(RA8875_MRWC);
tft.writeData(c);
}}

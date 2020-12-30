#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "RA8875.h"
#include "RA8875Register.h"

raspiRA8875 tft = raspiRA8875();

int main(void) {
tft.displaySpiBegin();
tft.displayBegin();
tft.displayOn(true);
tft.textMode();
tft.textEnlarge(0);
tft.setTextColor(RA8875_WHITE, RA8875_BLACK);
tft.setTextCursor(0,0);
tft.textWrite("RA8875 Demo");
tft.textEnlarge(1);
tft.setTextCursor(0,30);
tft.textWrite("RA8875 Demo");
tft.textEnlarge(2);
tft.setTextCursor(0,90);
tft.textWrite("RA8875 Demo");
tft.textEnlarge(3);
tft.setTextCursor(0,160);
tft.textWrite("RA8875 Demo");
}

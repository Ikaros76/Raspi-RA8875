#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "RA8875.h"
#include "RA8875Register.h"

int main(void) {
displaySpiBegin();
displayBegin();
displayOn(true);
textMode();
textEnlarge(0);
setTextColor(RA8875_WHITE, RA8875_BLACK);
setTextCursor(0,0);
textWrite("RA8875 Demo");
textEnlarge(1);
setTextCursor(0,30);
textWrite("RA8875 Demo");
textEnlarge(2);
setTextCursor(0,90);
textWrite("RA8875 Demo");
textEnlarge(3);
setTextCursor(0,160);
textWrite("RA8875 Demo");
}

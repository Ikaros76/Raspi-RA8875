#include <stdbool.h>
#include <stdint.h>

#include "RA8875.h"
#include "RA8875Register.h"

int main(void) {
displaySpiBegin();
displayBegin();
displayOn(true);
textMode();
setTextColor(RA8875_WHITE, RA8875_BLACK);
setTextCursor(0,0);
textWrite("RA8875 Demo");
setTextColor(RA8875_BLACK, RA8875_WHITE);
setTextCursor(0,30);
textWrite("RA8875 Demo");
setTextCursor(0,60);
textEnlarge(2);
textWrite("RA8875 Demo");
}

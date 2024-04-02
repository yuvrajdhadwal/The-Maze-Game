#include "gba.h"

u16 getBackgroundColor(int row, int col) {
    return *(videoBuffer + OFFSET(row, col, WIDTH));
}


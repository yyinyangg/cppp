#include "src/display.h"

#include "gfx.h"
#include "glcdfont.h"
#include <stdio.h>

static int16_t cursorX, cursorY;
static uint16_t textColor;
static uint8_t textSize;
static int textBackground;

uint16_t color565_s(const uint8_t r, const uint8_t g, const uint8_t b) {
    const uint8_t hiR = (r & 0xF8) >> 3;
    const uint8_t hiG = (g & 0xFC) >> 2;
    const uint8_t hiB = (b & 0xF8) >> 3;
    const uint16_t result = (hiR << 11) | (hiG << 5) | hiB;
    return result;
    // A more compact solution is:
    // return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void printPattern_s(const uint16_t backgroundColor, const uint16_t foregroundColor) {
    int j;
    uint8_t blocksize = 4;
    cppp_fillScreen(backgroundColor);
    for (int i = 0; i < 480; i += blocksize) {
        if (i % (blocksize * 2) == 0)
            j = 0;
        else
            j = blocksize;
        for (j; j < 320; j += blocksize * 2)
            cppp_fillRect(i, j, blocksize, blocksize, foregroundColor);
    }
}

void initCursor_s() {
    cursorX = 0;
    cursorY = 0;
    textColor = WHITE;
    textSize = 2;
    textBackground = BLACK;
}

void setCursor_s(const int16_t x, const int16_t y) {
    cursorX = x;
    cursorY = y;
}

void setTextColor_s(const uint16_t c) {
    // For 'transparent' background, we'll set the bg to the same as the rest of the display
    textColor = c;
}

void setTextSize_s(const uint8_t s) {
    textSize = (s > 0) ? s : 1;
}

void setBackgroundColor_s(const int bg) {
    textBackground = bg;
}

void drawChar_s(int x, int y, char c, int color, int bg, char size) {
    if ((x >= 480) ||               // Clip right
        (y >= 320) ||               // Clip top
        ((x + 6 * size - 1) < 0) || // Clip left
        ((y + 8 * size - 1) < 0)) { // Clip bottom
        return;
    }

    char i, j;
    for (i = 0; i < 6; i++) { // Draw in x-direction
        char line;            // Vertical line
        if (i < 5)
            line = font[(c * 5) + i]; // Save the i.x-line from (i,j) to (i,j+7) in the char line
        else
            line = 0x0;                       // Padding line toward next character
        for (j = 0; j < 8; j++, line >>= 1) { // Draw in y-direction
            if (line & 0x1) {
                if (size == 1)
                    cppp_drawPixel(x + i, y - j, color);
                else
                    cppp_fillRect(x + (i * size), y - ((j + 1) * size) + 1, size, size, color); //keep in mind cppp_fillRect wants the lower left corner
            }
            else if (bg != color) {
                if (size == 1)
                    cppp_drawPixel(x + i, y - j, bg);
                else
                    cppp_fillRect(x + (i * size), y - ((j + 1) * size) + 1, size, size, bg);
            }
        }
    }
}

void writeAuto_s(const char c) {
    if (c == '\n') {
        cursorY -= textSize * 8;
        cursorX = 0;
    }
    else if (c == '\r') {
        // nop
    }
    else {
        if (((cursorX + textSize * 6) > 480)) { // Heading off edge
            cursorX = 0;                        // Reset x to zero
            cursorY -= textSize * 8;            // Advance y one line
        }
        if (((cursorY - textSize * 6) < 0)) { // Heading off edge
            cursorY = 319;                    // Advance y one line
            cursorX = 0;                      // Reset x to zero
        }
        cursorX = textSize * 6;
        drawChar_s(cursorX, cursorY, c, textColor, textBackground, textSize);
        //cursorX += textSize * 6;
    }
}

void writeText_s(const char* text) {
    cursorY = 0;
    cursorX = 0;
    for (int i = 0; text[i] != 0; i++) {
        writeAuto_s(text[i]);
    }
}

void writeTextln_s(const char* text) {
    cursorY = textSize * 8;
    cursorX = 0;
    for (int i = 0; text[i] != 0; i++) {
        writeAuto_s(text[i]);
    }
    
}

void writeNumberOnDisplay_s(const uint8_t* value) {
    char buffer[4];
    sprintf(buffer, "%u", *value);
    writeText_s(buffer);
}

void writeNumberOnDisplayRight_s(const uint8_t* value) {
    int x = 100;
      char buffer[4];
      char* bufferStart = buffer;
      while(x > 1 && *value < x) {
          *bufferStart = ' ';
          bufferStart++;
          x /= 10;
      }
      sprintf(bufferStart, "%u", *value);
      writeTextln_s(buffer);
}

void write16BitNumberOnDisplay_s(const uint16_t* value, uint8_t mode) {
    int x = 10000;
    if (mode == 1) {
        uint8_t numberOfDigits = 6;
        while(x > 1 && *value < x) {
            numberOfDigits--;
          x /= 10;
        }
        char buffer[numberOfDigits];
        sprintf(buffer, "%u", *value);
        writeText_s(buffer);
    } else {
        char buffer[6];
        char* bufferStart = buffer;
        while(x > 1 && *value < x) {
            *bufferStart = ' ';
            bufferStart++;
            x /= 10;
        }
        sprintf(bufferStart, "%u", *value);
        writeText_s(buffer); 
    }
}

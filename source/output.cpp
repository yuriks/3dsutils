#include "output.h"

#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdarg>
#include <cstdio>

#include <3ds.h>

#include "text.h"

static std::string bufferTop;
static std::string bufferBottom;

static int countLines(const std::string& str)
{
    if (str.empty())
        return 0;

    return 1 + std::count_if(str.begin(), str.end(), [](char c) { return c == '\n'; });
}

static void deleteFirstLine(std::string* str)
{
    if (str->empty())
        return;
    
    size_t linebreak = str->find_first_of('\n');
    
    if (linebreak == std::string::npos || linebreak + 1 > str->length()) {
        *str = {};
        return;
    }
    
    *str = str->substr(linebreak + 1);
}

static void drawFrame(gfxScreen_t screen, char b, char g, char r)
{
    int screenHeight = 240;
    int screenWidth = (screen == GFX_TOP) ? 400 : 320;
    std::string& textBuffer = (screen == GFX_TOP) ? bufferTop : bufferBottom;

    u8* bufAdr = gfxGetFramebuffer(screen, GFX_LEFT, nullptr, nullptr);
    for (int i = 0; i < screenWidth * screenHeight * 3; i += 3) {
        bufAdr[i]   = b;
        bufAdr[i+1] = g;
        bufAdr[i+2] = r;
    }

    int lines = countLines(textBuffer);
    while (lines > (screenHeight / fontDefault.height - 3)) {
        deleteFirstLine(&textBuffer);
        lines--;
    }
    gfxDrawText(screen, GFX_LEFT, nullptr, textBuffer, screenHeight - fontDefault.height * 3, 10);
}

void drawFrames()
{
    drawFrame(GFX_TOP, 0x88, 0x66, 0x00);
    drawFrame(GFX_BOTTOM, 0x00, 0x00, 0x00);
    gfxFlushBuffers();
    gfxSwapBuffers();
}

void print(gfxScreen_t screen, const char* format, ...)
{
    std::string& textBuffer = (screen == GFX_TOP) ? bufferTop : bufferBottom;
    
    va_list arguments;
    char *vaStr;

    va_start(arguments, format);
    vasprintf(&vaStr, format, arguments);
    va_end(arguments);

    textBuffer += std::string(vaStr);
    svcOutputDebugString(vaStr, strlen(vaStr));
    free(vaStr);

    drawFrames();
}

void clearScreen(gfxScreen_t screen)
{
    std::string& textBuffer = (screen == GFX_TOP) ? bufferTop : bufferBottom;
    textBuffer.clear();
    drawFrames();
}

void clearScreens()
{
    clearScreen(GFX_TOP);
    clearScreen(GFX_BOTTOM);
}

#pragma once

struct Glyph {
    // Glyph representation
    char c;

    // x and y origin of the character.
    int x, y;

    // width and height in pixels.
    int w, h;

    // x and y offset
    int xo, yo;

    // Pixels after this character to begin
    // drawing the next one.
    int xa;

    // Glyph data.
    u8* data;
};

struct font_s {
    u8* data;
    Glyph* desc;
    u8 height;
    u8 color[3];
};

extern u8 font1Data[];
extern Glyph font1Desc[];

extern font_s fontDefault;

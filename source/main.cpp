// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <stdlib.h>
#include <stdio.h>
#include <3ds.h>

#include "output.h"
#include "utils/shared_font/shared_font.h"

static unsigned int util_counter = 0;
static void (*utils[]) (void) = {
    SharedFont::Dump,
};

int main()
{
    srvInit();
    aptInit();
    hidInit(NULL);
    fsInit();
    gfxInit();
    gfxSet3D(false);

    clearScreens();
    print(GFX_TOP, "Press A to begin...\n");

    while (aptMainLoop()) {
        drawFrames();

        hidScanInput();
        if (hidKeysDown() & KEY_START) {
            break;
        } else if (hidKeysDown() & KEY_A) {
            clearScreen(GFX_TOP);

            if (util_counter < (sizeof(utils) / sizeof(utils[0]))) {
                utils[util_counter]();
                util_counter++;
            } else {
                break;
            }

            print(GFX_TOP, "\n");
            print(GFX_TOP, "Press A to continue...\n");
        }

        gspWaitForEvent(GSPEVENT_VBlank0, false);
    }

    clearScreens();

    gfxExit();
    fsExit();
    hidExit();
    aptExit();
    srvExit();
    return 0;
}

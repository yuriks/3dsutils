// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <3ds.h>

#include "output.h"
#include "utils/shared_font/shared_font.h"

namespace SharedFont {

static const u32 SHARED_FONT_SIZE = 0x300000;

void Dump() {
    static const char* path = "/shared_font.bin";
    
    print(GFX_TOP, "Dumping shared system font (%s)... ", path);
    
    // Connect to APT service...
    
    Handle apt_handle;
    srvGetServiceHandle(&apt_handle, "APT:U");
    u32* cmdbuf=getThreadCommandBuffer();
    
    // Call APT::GetSharedFont function to load font into memory...
    
    cmdbuf[0] = 0x00440000;
    svcSendSyncRequest(apt_handle);

    Handle mem_handle = cmdbuf[4];
    void* shared_font_addr = reinterpret_cast<void*>(cmdbuf[2]);
    
    // Close APT handle...
    
    svcCloseHandle(apt_handle);
    
    // Map shared font memory...
    
    svcMapMemoryBlock(mem_handle, 0, MEMPERM_READ, MEMPERM_DONTCARE);
    
    // Dump shared font to SDMC...
    
    Handle               file_handle;
    u32                  bytes_written = 0;
    static const FS_path fs_path       = FS_makePath(PATH_CHAR, path);
    FS_archive           sdmc_archive  = (FS_archive) { 0x00000009, { PATH_EMPTY, 1, (u8*) "" } };
    
    FSUSER_OpenArchive(NULL, &sdmc_archive);
    FSUSER_OpenFile(NULL, &file_handle, sdmc_archive, fs_path, FS_OPEN_CREATE | FS_OPEN_WRITE, FS_ATTRIBUTE_NONE);
    Result res = FSFILE_Write(file_handle, &bytes_written, 0x0, shared_font_addr, SHARED_FONT_SIZE, FS_WRITE_FLUSH);
    FSFILE_Close(file_handle);
    FSUSER_CloseArchive(NULL, &sdmc_archive);
    
    // Check result...
    
    if (res == 0 && bytes_written == SHARED_FONT_SIZE)
        print(GFX_TOP, "Done!\n");
    else
        print(GFX_TOP, "Failed!\n");
}

} // namespace

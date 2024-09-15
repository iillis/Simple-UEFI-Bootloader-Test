#ifndef GOP_H
#define GOP_H

#include <efi.h>

typedef struct {
    UINT64 FramebufferBase;
    UINT64 Width;
    UINT64 Height;
    UINT64 Pitch;
} FramebufferInfo;

VOID GetFramebufferInfo(FramebufferInfo * Framebuffer);

#endif
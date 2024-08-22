#ifndef GOP_H
#define GOP_H

#include <efi_types.h>

typedef struct {
    UINT64 FramebufferBase;
    UINT64 Width;
    UINT64 Height;
    UINT64 Pitch;
} FramebufferInfo;

VOID GetFramebufferInfo(FramebufferInfo * Framebuffer);

#endif
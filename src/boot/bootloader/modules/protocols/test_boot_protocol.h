#ifndef TEST_BOOT_PROTOCOL_H
#define TEST_BOOT_PROTOCOL_H

#include <efi.h>
#include <modules/gop/gop.h>
typedef struct {
    FramebufferInfo * Framebuffer;
} BootInfoTest;

#endif
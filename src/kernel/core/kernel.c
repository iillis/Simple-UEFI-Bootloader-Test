#include <modules/protocols/test_boot_protocol.h>
#include <serial/serial.h>

VOID _start(BootInfoTest * BootInfo) {
    CHAR8 String[] = "Serial Test\n";
    SerialPrint(String);

    for (UINTN i = 0; i < 1000; i++) {
        volatile UINT32 *FramebufferPointer = (UINT32 *) BootInfo->Framebuffer->FramebufferBase;
        FramebufferPointer[i * (BootInfo->Framebuffer->Pitch) + i] = 0xFFFFFF;
    }
    
}

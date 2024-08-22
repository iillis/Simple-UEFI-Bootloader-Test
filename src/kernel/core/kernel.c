#include <serial/serial.h>
#include <utilities/utilities.h>

typedef struct {
    UINT64 FramebufferBase;
    UINT64 Width;
    UINT64 Height;
    UINT64 Pitch;
} FramebufferInfo;


VOID _start(FramebufferInfo * Framebuffer) {
    SerialPrint("\nHELLO FROM THE KERNEL!\n");
    for (UINT16 i = 0; i < 100; i++) {
        CHAR8 String[20];
        Itoa(Framebuffer->Width, String);
        SerialPrint(String);
        SerialPrint("\n");
    }
}

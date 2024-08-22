#include "serial.h"
#include <efi_types.h>

#define COM1 0x3f8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8 

VOID PortOutB(UINT16 Port, UINT8 Data) {
    asm("out %%al, %%dx" : : "a"(Data), "d"(Port));
}

UINT8 PortInB(UINT16 Port) {
    UINT8 Return;
    asm("in %%dx, %%al" : "=a"(Return) : "d"(Port));
    return Return;
}

VOID WaitForTransmit() {
    while (!(PortInB(COM1 + 5) & 0x20)) { asm("pause"); }
}

VOID SerialPrint(CHAR8 *String) {
    while (*String != '\0') {
        WaitForTransmit();
        PortOutB(COM1, *String);
        String++;
    }
}
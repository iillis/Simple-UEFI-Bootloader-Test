#include "gop.h"
#include <efi.h>
#include <bootloader.h>
#include <wrappers/wrappers.h>
#include <terminal/terminal.h>

VOID GetFramebufferInfo(FramebufferInfo * Framebuffer) {
    EFI_STATUS Status = 0;
    EFI_GUID GopProtocol = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL * Gop = NULL;
    Status = SystemTable->BootServices->LocateProtocol(&GopProtocol, NULL, (VOID **)&Gop);
    CheckError(Status, L"Failed to locate the Graphic Output Protocol\n\r", L"Succesfully located the Graphic Output Protocol\n\r");
      
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN SizeOfInfo, NumModes, NativeMode;

    Status = Gop->QueryMode(Gop, Gop->Mode==NULL?0:Gop->Mode->Mode, &SizeOfInfo, &info);
    if (Status == EFI_NOT_STARTED) {
        Status = Gop->SetMode(Gop, 0);
    }
    else {
        CheckError(Status, L"Failed to query the GOP mode\n\r", L"Succesfully queried the GOP mode\n\r");
        NativeMode = Gop->Mode->Mode;
        NumModes = Gop->Mode->MaxMode;
    }
    /*
    for (UINT64 i = 0; i < NumModes; i++) {
        Status = Gop->QueryMode(Gop, i, &SizeOfInfo, &info);
        PrintHex(Gop->Mode->FrameBufferBase);
        PrintInt(Gop->Mode->FrameBufferSize);
        PrintInt(Gop->Mode->Info->HorizontalResolution);
        PrintInt(Gop->Mode->Info->VerticalResolution);
        PrintInt(Gop->Mode->Info->PixelsPerScanLine);
    }
    */
    Framebuffer->FramebufferBase = Gop->Mode->FrameBufferBase;
    Framebuffer->Width = Gop->Mode->Info->HorizontalResolution;
    Framebuffer->Height = Gop->Mode->Info->VerticalResolution;
    Framebuffer->Pitch = Gop->Mode->Info->PixelsPerScanLine;

    return;
}
#include "wrappers.h"
#include <efi_types.h>
#include <efi_system.h>
#include "../terminal/terminal.h"
#include "../bootloader.h"

VOID WaitForKeyPress() {
        UINTN KeyEvent = 0;
        EFI_INPUT_KEY Key;
        SystemTable->BootServices->WaitForEvent(1,&SystemTable->ConIn->WaitForKey,&KeyEvent);
        SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
}

VOID CheckError(EFI_STATUS Status, CHAR16 * ErrorMessage, CHAR16 * SuccessMessage) {
    if(Status != EFI_SUCCESS){
        SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_RED | EFI_BACKGROUND_BLACK);
        Print(L"Error:\t");
        switch ((int)Status)
        {
        case EFI_BUFFER_TOO_SMALL:
            Print(L"Buffer too small\n\r");
            break;
        case EFI_INVALID_PARAMETER:
            Print(L"Invalid Parameters\n\r");
            break;
        default:
            break;
        }
        Print(ErrorMessage);
        Print(L"press any key to proceed...\n\r");
        WaitForKeyPress();
        Exit();
    }
    Print(SuccessMessage);
    return;
}

VOID Error(EFI_STATUS Status, CHAR16 * ErrorMessage) {
    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_RED | EFI_BACKGROUND_BLACK);
    Print(L"Error:\t");
    switch((int)Status)
    {
    case EFI_BUFFER_TOO_SMALL:
        Print(L"Buffer too small\n\r");
        break;
    case EFI_INVALID_PARAMETER:
        Print(L"Invalid Parameters\n\r");
        break;
    default:
        Print(L"\n\r");
        break;
    }
    Print(ErrorMessage);
    Print(L"press any key to proceed...\n\r");
    WaitForKeyPress();
    Exit();
}

VOID Exit(VOID) {
    SystemTable->BootServices->Exit(ImageHandle, 0, 0, 0);
}
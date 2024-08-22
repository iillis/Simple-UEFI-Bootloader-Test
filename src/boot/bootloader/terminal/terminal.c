#include "terminal.h"
#include <bootloader.h>
#include <utilities/utilities.h>

VOID Print(CHAR16 * text) {
    SystemTable->ConOut->OutputString(SystemTable->ConOut,text);
}

VOID Newline(VOID) {
    Print(L"\n\r");
}

VOID PrintHex(UINT64 Hex) {
    Print(L"0x");
    Print(U64ToHex(Hex));
    Newline();
}

VOID PrintInt(UINT64 Integer) {
    CHAR16 String[30];
    Itoa16(Integer, String);
    Print(String);
    Newline();
}

VOID PrintHexPrefix(UINT64 Hex, CHAR16 * text) {
    Print(text);
    PrintHex(Hex);
}

VOID PrintIntPrefix(UINT64 Integer, CHAR16 * text) {
    Print(text);
    PrintInt(Integer);
}
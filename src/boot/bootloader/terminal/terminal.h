#ifndef EFI_ITOA_H
#define EFI_ITOA_H

#include <efi.h>

VOID Print(CHAR16 * text);
VOID Newline(VOID);
VOID PrintHex(UINT64 Hex);
VOID PrintInt(UINT64 Integer);
VOID PrintHexPrefix(UINT64 Hex, CHAR16 * text);
VOID PrintIntPrefix(UINT64 Integer, CHAR16 * text);
#endif
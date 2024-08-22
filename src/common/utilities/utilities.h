#ifndef UTILITIES_H
#define UTILITIES_H

#include <efi_types.h>

CHAR16* U64ToHex(UINT64 Input);
VOID Itoa(INT Number, CHAR8 String[]);
VOID Itoa16(INT Number, CHAR16 String[]);

#endif
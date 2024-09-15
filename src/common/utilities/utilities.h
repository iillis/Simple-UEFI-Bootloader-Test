#ifndef UTILITIES_H
#define UTILITIES_H

#include <efi_types.h>

VOID Itoa16(INT Number, CHAR16 String[]);
VOID Itoa(INT Number, CHAR8 String[]);
CHAR16* U64ToHex(UINT64 Input);


#endif
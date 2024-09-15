#include "utilities.h"
#include <efi_types.h>
#include <memory/memory.h>

VOID Itoa16(INT Number, CHAR16 String[]) {
     int i, Sign;
 
     if ((Sign = Number) < 0)
         Number = -Number;
     i = 0;
     do {
         String[i++] = Number % 10 + '0';
     } while ((Number /= 10) > 0);
     if (Sign < 0)
         String[i++] = '-';
     String[i] = '\0';
     Strrev16(String);
}

VOID Itoa(INT Number, CHAR8 String[]) {
     int i, Sign;
 
     if ((Sign = Number) < 0)
         Number = -Number;
     i = 0;
     do {
         String[i++] = Number % 10 + '0';
     } while ((Number /= 10) > 0);
     if (Sign < 0)
         String[i++] = '-';
     String[i] = '\0';
     Strrev(String);
}

CHAR16* U64ToHex(UINT64 Input) {
    UINT8 Limit = 19;
    static CHAR16 String[20];

    while (Input)
    {
        --Limit;
        String[Limit] = (Input%16)+((Input%16 > 9)? 0x37 :'0');
        Input/=16;
    }
    String[19] ='\0';
    return &String[Limit];
}
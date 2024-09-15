#ifndef EFI_MEMORY_H
#define EFI_MEMORY_H

#include <efi_types.h>

VOID *Memcpy(VOID *dest, CONST VOID *src, UINTN n);
VOID *Memset(VOID *s, INT c, UINTN n);
VOID *Memmove(VOID *dest, CONST VOID *src, UINTN n);
INT Memcmp(CONST VOID *s1, CONST VOID *s2, UINTN n);
INT Strncmp(CONST CHAR8 *s1, CONST CHAR8 *s2, UINTN n);
INT Strcmp(CONST CHAR8 *s1, CONST CHAR8 *s2);
CHAR8 * Strrev(CHAR8 String[]);
CHAR16 * Strrev16(CHAR16 String[]);
UINTN Strlen(const char *str);
UINTN Strlen16(const CHAR16 *str);

#endif
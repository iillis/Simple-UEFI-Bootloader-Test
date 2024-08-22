#ifndef EFI_WRAPPERS_H
#define EFI_WRAPPERS_H

#include <efi_types.h>
#include <efi_system.h>

VOID WaitForKeyPress(void);
INT CheckError(EFI_STATUS Status, CHAR16 * ErrorMessage, CHAR16 * SuccessMessage);
VOID Error(EFI_STATUS Status, CHAR16 * ErrorMessage);
VOID Exit(VOID);

#endif
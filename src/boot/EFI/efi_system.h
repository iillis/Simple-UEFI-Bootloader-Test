#ifndef __EFI_SYSTEM_H
#define __EFI_SYSTEM_H

#include "efi.h"
#include "efi_text_protocol.h"
#include "efi_boot_services.h"
#include "efi_runtime_services.h"

typedef struct EFI_SYSTEM_TABLE{
    EFI_TABLE_HEADER                Hdr;
    CHAR16                         *FirmwareVendor;
    UINT32                          FirmwareRevision;
    EFI_HANDLE                      ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConIn;
    EFI_HANDLE                      ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*ConOut;
    EFI_HANDLE                      StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*StdErr;
    EFI_RUNTIME_SERVICES           *RuntimeServices;
    EFI_BOOT_SERVICES              *BootServices;
    UINTN                           NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE        *ConfigurationTable;
} EFI_SYSTEM_TABLE;

#endif
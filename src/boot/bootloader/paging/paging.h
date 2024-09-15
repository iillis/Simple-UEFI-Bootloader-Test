#ifndef EFI_PAGING_H
#define EFI_PAGING_H

#include <efi.h>

#define PAGE_SIZE 4096

#define ENTRIES_PER_TABLE 512

typedef struct {
    UINT64 Entries[ENTRIES_PER_TABLE];
} PAGE_TABLE;

extern PAGE_TABLE* Pml4;
extern UINT64 FreeEntry;

VOID SetupPaging();
VOID MapPage(PAGE_TABLE* Pml4, UINT64 VirtualAddress, UINT64 PhysicalAddress, UINT64 Flags, UINT8** memory);
#endif
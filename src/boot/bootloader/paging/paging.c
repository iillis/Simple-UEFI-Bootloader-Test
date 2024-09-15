#include "paging.h"
#include <efi.h>
#include <memory/memory.h>
#include <terminal/terminal.h>
#include <bootloader.h>
#include <wrappers/wrappers.h>
#include <cpu/cpu.h>

PAGE_TABLE* Pml4 = NULL;
UINT64 FreeEntry = 0;

PAGE_TABLE* InitPageTables(UINT8** Memory) {
    PAGE_TABLE* Table = (PAGE_TABLE*)(*Memory);
    for(UINT64 i = 0; i<= sizeof(PAGE_TABLE);i+=8){
        (*Memory)[i] = 0;
    }
    *Memory += sizeof(PAGE_TABLE);
    return Table;
}

VOID MapPage(PAGE_TABLE* Pml4, UINT64 VirtualAddress, UINT64 PhysicalAddress, UINT64 Flags, UINT8** Memory) {
    int Pml4Index = (VirtualAddress >> 39) & 0x1FF;
    int PdpIndex = (VirtualAddress >> 30) & 0x1FF;
    int PdIndex = (VirtualAddress >> 21) & 0x1FF;
    int PtIndex = (VirtualAddress >> 12) & 0x1FF;
    if (!(Pml4->Entries[Pml4Index] & 1)) {
        Pml4->Entries[Pml4Index] = (UINT64)InitPageTables(Memory) | Flags;
    }
    PAGE_TABLE* Pdp = (PAGE_TABLE*)(Pml4->Entries[Pml4Index] & ~0xFFF);

    if (!(Pdp->Entries[PdpIndex] & 1)) {
        Pdp->Entries[PdpIndex] = (UINT64)InitPageTables(Memory) | Flags;
    }
    PAGE_TABLE* Pd = (PAGE_TABLE*)(Pdp->Entries[PdpIndex] & ~0xFFF);

    if (!(Pd->Entries[PdIndex] & 1)) {
        Pd->Entries[PdIndex] = (UINT64)InitPageTables(Memory) | Flags;
    }
    PAGE_TABLE* Pt = (PAGE_TABLE*)(Pd->Entries[PdIndex] & ~0xFFF);

    Pt->Entries[PtIndex] = PhysicalAddress | Flags;
}

VOID SetupPaging() {
    /*
    *   GETTING MEMORY MAP
    */

    EFI_MEMORY_DESCRIPTOR TemporaryMemoryMap[1];
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
    EFI_STATUS Status = 0;

    UINTN MapKey = 0;
    UINTN EfiDescriptorSize = 0;
    UINT32 EfiDescriptorVersion = 0;
    UINTN MemmapSize = 0;

    MemmapSize = sizeof(TemporaryMemoryMap);
    SystemTable->BootServices->GetMemoryMap(&MemmapSize, TemporaryMemoryMap, &MapKey, &EfiDescriptorSize, &EfiDescriptorVersion);

    MemmapSize += 4096;

    Status = SystemTable->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, (MemmapSize/PAGE_SIZE), (EFI_PHYSICAL_ADDRESS *)&MemoryMap);
    CheckError(Status, L"Failed to allocate the memory map pool\n\r", L"Succesfully allocated the memory map pool\n\r");
    
    Status = SystemTable->BootServices->GetMemoryMap(&MemmapSize, MemoryMap, &MapKey, &EfiDescriptorSize, &EfiDescriptorVersion);
    CheckError(Status, L"Failed to get memory map\n\r", L"Succesfully got memory map\n\r");
    
    UINTN MemoryMapEntriesCount = MemmapSize/EfiDescriptorSize;
    PrintIntPrefix((UINT64)MemmapSize,L"Memory map size:\t");
    PrintIntPrefix((UINT64)EfiDescriptorSize,L"Memory map descriptor size:\t");
    PrintIntPrefix((UINT64)(MemoryMapEntriesCount),L"Memory map entries:\t");

    /*
    *   CALCULATING PAGES
    */

    EFI_MEMORY_DESCRIPTOR *LastEntry = NULL;
    for(UINTN i = MemoryMapEntriesCount-1; i>= 0; i--) {
        LastEntry = (VOID *)MemoryMap + i * EfiDescriptorSize;
        if( LastEntry->Type != EfiReservedMemoryType &&
            LastEntry->Type != EfiUnusableMemory &&
            LastEntry->Type != EfiMemoryMappedIO &&
            LastEntry->Type != EfiMemoryMappedIOPortSpace &&
            LastEntry->Type != EfiUnacceptedMemoryType) {
                break;
            }
    }
    UINT64 MemorySize = (UINT64)(LastEntry->PhysicalStart) + LastEntry->NumberOfPages * PAGE_SIZE;
    UINT64 PageCount = MemorySize / PAGE_SIZE;
    UINT64 PageTables = PageCount/512 + ((PageCount%512)?1:0);
    UINT64 PageDirectories = PageTables/512 + ((PageTables%512)?1:0);
    UINT64 PDPT = PageDirectories/512 + ((PageDirectories%512)?1:0);
    UINT64 PML4 = PDPT/512 + ((PDPT%512)?1:0);
    
    UINT64 PagingStructTotalCount = PageTables + PageDirectories + PDPT + PML4;

    PrintIntPrefix(PagingStructTotalCount, L"Number of kb to store:\t");
    PrintIntPrefix(MemorySize/1073741824, L"Memory Size (GB): \t");
    
    for(UINTN i = 0; i < MemoryMapEntriesCount; i++) {
        EFI_MEMORY_DESCRIPTOR *Entry = (VOID *)MemoryMap + i * EfiDescriptorSize;
        if(FreeEntry == 0 && Entry->Type == EfiConventionalMemory && Entry->NumberOfPages >= PagingStructTotalCount){
            FreeEntry = (UINT64)Entry->PhysicalStart;
            goto Success;
        }
    }
    Error(0,L"FAILED TO LOCATE A VALID ENTRY\n\r");

    Success:
    Pml4 = InitPageTables((UINT8 **)&FreeEntry);
    for (UINT64 i = 0; i < PageCount; i++) {
        MapPage(Pml4, i * PAGE_SIZE, i * PAGE_SIZE, 0x3, (UINT8 **)&FreeEntry);
    }

Retry:
    UINTN Retries = 0;
    Print(L"Loading tables...\n\r");
    //remove to see infos
    //WaitForKeyPress();
    Status = SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK); //The white is to make serial visible
    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
    MemmapSize += 4096;
    Status = SystemTable->BootServices->GetMemoryMap(&MemmapSize, MemoryMap, &MapKey, &EfiDescriptorSize, &EfiDescriptorVersion);
    if (Retries == 0 && Status) {
        CheckError(Status, L"Failed to get memory map\n\r", L"Succesfully got memory map\n\r");
    }
    Status = SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
    if (Status) {
        if (Retries == 128) {
            CheckError(Status, L"Failed to exit boot services\n\r", L"Succesfully exited boot services\n\r");
        }
        Retries++;
        goto Retry;
    }
    asm volatile("mov %0, %%cr3" : : "r"((Pml4)) : "memory");
    return;
}
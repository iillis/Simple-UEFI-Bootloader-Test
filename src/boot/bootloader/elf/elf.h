#ifndef EFI_ELF_H
#define EFI_ELF_H

#include <efi.h>

#define PT_LOAD 0x00000001

typedef struct {
    UINT8 Ident[16];
    UINT16 Type;
    UINT16 Machine;
    UINT32 Version;
    UINT64 Entry;          
    UINT64 ProgramHeaderOffset;
    UINT64 SectionHeaderOffset;
    UINT32 Flags;                   
    UINT16 HeaderSize;  
    UINT16 ProgramHeaderTableEntrySize; 
    UINT16 ProgramHeaderTableEntryCount; 
    UINT16 SectionHeaderTableEntrySize;
    UINT16 SectionHeaderTableEntryCount;
    UINT16 SectionHeaderStringTableIndex;
} ELF64HDR;

typedef struct {
    UINT32 Type;
    UINT32 Flags;
    UINT64 Offset;
    UINT64 VirtualAddress;
    UINT64 PhysicalAddress;
    UINT64 FileSize;
    UINT64 MemorySize;
    UINT64 Align;
} ELF64PHDR;

typedef struct {
    UINT32 Name;
    UINT32 Type;
    UINT64 Flags;
    UINT64 Address;
    UINT64 Offset;
    UINT64 Size;
    UINT32 Link;
    UINT32 Ifno;
    UINT64 Align;
    UINT64 EntriesSize;
} ELF64SHDR;

VOID DumpHeader(ELF64HDR Header);

#endif
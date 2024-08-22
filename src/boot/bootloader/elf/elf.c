#include "elf.h"
#include <terminal/terminal.h>
#include <bootloader.h>

VOID DumpHeader(ELF64HDR Header) {
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_YELLOW | EFI_BACKGROUND_BLUE);
	Print(L"HEADER FILE INFO:\n\r");
    PrintHexPrefix(Header.Entry, L"Entry:\t");
    PrintIntPrefix(Header.Flags, L"Flags:\t");
    PrintIntPrefix(Header.HeaderSize, L"Size(bytes):\t");
    PrintIntPrefix(Header.Machine, L"Machine:\t");
    PrintIntPrefix(Header.ProgramHeaderTableEntryCount, L"Program header table entries count:\t");
    PrintIntPrefix(Header.ProgramHeaderOffset, L"Program header offset:\t");
    PrintIntPrefix(Header.ProgramHeaderTableEntrySize, L"Program header table entry size(bytes):\t");
    PrintIntPrefix(Header.SectionHeaderTableEntryCount, L"Section header table entries count:\t");
    PrintIntPrefix(Header.SectionHeaderOffset, L"Section header offset:\t");
    PrintIntPrefix(Header.SectionHeaderTableEntrySize, L"Section header table entry size(bytes):\t");
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLUE);
    return;
}
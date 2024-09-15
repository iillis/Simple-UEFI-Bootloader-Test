#include "bootloader.h"
#include <efi.h>
#include <memory/memory.h>
#include <paging/paging.h>
#include <wrappers/wrappers.h>
#include <terminal/terminal.h>
#include <elf/elf.h>
#include <cpu/cpu.h>
#include <modules/gop/gop.h>
#include <modules/protocols/test_boot_protocol.h>

EFI_SYSTEM_TABLE * SystemTable;
EFI_HANDLE ImageHandle = 0;

static UINT8 Stack[PAGE_SIZE * 16];
static UINT64 StackBase = (UINT64)(Stack + PAGE_SIZE * 16);
static UINT64 StackTop = (UINT64)Stack;

EFI_STATUS EFIAPI EfiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE * systemTable) {
    SystemTable = systemTable;
    ImageHandle = &imageHandle;
    EFI_STATUS Status;

    /*
    *   SETTING UP TERMINAL AND PRINTING WELCOME MESSAGE
    */
    Status = SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_WHITE | EFI_BACKGROUND_BLUE);
	Status = SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	Print(L"booting...\n\r");

    /*
    *   GETTING THE KERNEL FILE
    */
   
   	EFI_FILE_PROTOCOL* Kernel;
	EFI_FILE_PROTOCOL* file;

    EFI_GUID SafeFsProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* SafeFsProtocol = NULL;
    EFI_FILE_PROTOCOL* RootDirectory = NULL;

    SystemTable->BootServices->LocateProtocol(&SafeFsProtocolGuid, 0, (VOID**)&SafeFsProtocol);
    SafeFsProtocol->OpenVolume(SafeFsProtocol, &RootDirectory);

    CHAR16 * KernelPath = L"\\EFI\\BOOT\\kernel.elf";
	Status = RootDirectory->Open(RootDirectory, &Kernel, KernelPath, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0 );
    CheckError(Status, L"KERNEL FILE DOES NOT EXIST\n\r", L"KERNEL FILE LOCATED\n\r");

    /*
    *   PARSING THE ELF HEADER
    */ 

	ELF64HDR * Header;
    UINTN HeaderSize = 0;
    UINTN FileInfoSize = 0;
    EFI_FILE_INFO * FileInfo = NULL;

    VOID * KernelImage = NULL;

    //GETTING FILE SIZE
    Kernel->GetInfo(Kernel,&gEfiFileInfoGuid,&FileInfoSize, NULL);

    Status = SystemTable->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData,FileInfoSize/4096 + (FileInfoSize%4096)?1:0,(EFI_PHYSICAL_ADDRESS *)&FileInfo);
    CheckError(Status, L"FAILED TO ALLOCATE MEMORY FOR FILE INFO\n\r", L"SUCCESSFULLY ALLOCATED MEMORY FOR FILE INFO\n\r");

    Status = Kernel->GetInfo(Kernel,&gEfiFileInfoGuid,&FileInfoSize,FileInfo);
    CheckError(Status, L"FAILED TO GET KERNEL INFO\n\r", L"SUCCESSFULLY GOT KERNEL INFO\n\r");

    Status = SystemTable->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData,FileInfo->FileSize/4096+1,(EFI_PHYSICAL_ADDRESS *)&KernelImage);
    CheckError(Status, L"FAILED TO ALLOCATE MEMORY FOR THE KERNEL IMAGE\n\r", L"SUCCESSFULLY ALLOCATED MEMORY THE KERNEL IMAGE\n\r");

    Kernel->Read(Kernel, &FileInfo->FileSize, KernelImage);
    Header = (ELF64HDR *)KernelImage;

    //VALIDATING HEADER
    Status = Strncmp((char *)Header->Ident, "\177ELF", 4);
    CheckError(Status, L"INVALID KERNEL ELF HEADER\n\r", L"VALID KERNEL ELF FILE HEADER\n\r");

    UINT64 MinVirtualAddress;
    UINT64 MaxVirtualAddress;
    DumpHeader(*Header);
    for (UINT16 i = 0; i < Header->ProgramHeaderTableEntryCount; i++) {
        ELF64PHDR *Phdr = KernelImage + (Header->ProgramHeaderOffset + i * Header->ProgramHeaderTableEntrySize);
        if(i == 0) {
            MinVirtualAddress = Phdr->VirtualAddress;
        }
        MaxVirtualAddress = Phdr->VirtualAddress + Phdr->MemorySize;
    }
    UINT64 ImageSize = MaxVirtualAddress - MinVirtualAddress;

    PrintHexPrefix(MinVirtualAddress,L"Base:\t");
    PrintHexPrefix(MaxVirtualAddress,L"End:\t");
    PrintIntPrefix(ImageSize,L"Size(bytes):\t");

    /*
    *   COPYING THE KERNEL
    */ 

    VOID * EfiCopy = NULL;
    Status = SystemTable->BootServices->AllocatePages(AllocateAnyPages,EfiLoaderData,ImageSize/PAGE_SIZE+1, (EFI_PHYSICAL_ADDRESS *)&EfiCopy);
    CheckError(Status, L"FAILED TO ALLOCATE MEMORY FOR THE KERNEL IMAGE\n\r", L"SUCCESSFULLY ALLOCATED MEMORY THE KERNEL IMAGE\n\r");

    for (UINT16 i = 0; i < Header->ProgramHeaderTableEntryCount; i++) {
        ELF64PHDR *Phdr = KernelImage + (Header->ProgramHeaderOffset + i * Header->ProgramHeaderTableEntrySize);
        if (Phdr->Type == PT_LOAD || Phdr->MemorySize != 0) {
            Memcpy(EfiCopy+(Phdr->VirtualAddress - 0xFFFFFFFF80000000),  (VOID *)(KernelImage) + Phdr->Offset, Phdr->FileSize);
        }
    }

    /*
    *   SETTING UP PAGING
    */
    FramebufferInfo Framebuffer;
    GetFramebufferInfo(&Framebuffer);
    SetupPaging();
    for (UINT64 i = 0; i < ImageSize/PAGE_SIZE + 1; i++) {
        MapPage(Pml4, 0xFFFFFFFF80000000 + i * PAGE_SIZE, (UINT64)EfiCopy + i * PAGE_SIZE, 3, (UINT8 **) &FreeEntry);
    }

    SetStackPointer(StackBase);
    
    /*
    *   JUMPING TO THE KERNEL
    */
   
    BootInfoTest BootInfo = { .Framebuffer = &Framebuffer};
    __attribute__((sysv_abi)) VOID (*Entry)(BootInfoTest * BootInfoStruct) = (__attribute__((sysv_abi)) VOID (*)(BootInfoTest * BootInfoStruct))Header->Entry;
    Entry(&BootInfo);
    for(;;);
}



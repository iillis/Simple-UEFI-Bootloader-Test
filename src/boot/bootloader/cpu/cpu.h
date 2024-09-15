#ifndef EFI_CPU_H
#define EFI_CPU_H

#include <efi.h>

static inline VOID SetCr3(UINT64 Value) {
    asm volatile (
        "mov %0, %%cr3"
        :
        : "r"(Value)
        : "memory"
    );
}

static inline VOID SetStackPointer(UINT64 StackPointer) {
    asm volatile (
        "mov %0, %%rsp"
        :
        : "r" (StackPointer)
        : "rsp"
    );
}

static inline VOID SetStackBase(UINT64 StackBase) {
    __asm__ volatile (
        "mov %0, %%rbp"
        :
        : "r" (StackBase)
        : "rbp"
    );
}
#endif
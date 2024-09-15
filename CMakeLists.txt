cmake_minimum_required(VERSION 3.29)
project(SUBT C CXX ASM_NASM)
enable_language(ASM_NASM)

set(CMAKE_C_STANDARD 23)

set(CMAKE_SYSTEM_NAME Generic)

set(target_triple x86_64-unknown-none-elf)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET     ${target_triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET   ${target_triple})

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

add_link_options(-fuse-ld=lld)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
set(IMAGE_OUTPUT_NAME "output")
set(KERNEL_OUTPUT_NAME "kernel")

#BOOTLOADER
file(GLOB_RECURSE BOOTLOADER_FILES CONFIGURE_DEPENDS src/boot/*.c src/boot/*.h src/boot/*.asm src/boot/*.cpp src/boot/*.hpp src/common/*)

add_executable(SRC_BOOTLOADER ${BOOTLOADER_FILES})

set_target_properties(SRC_BOOTLOADER PROPERTIES OUTPUT_NAME BOOTX64.EFI)

target_include_directories(SRC_BOOTLOADER PRIVATE src/boot/bootloader/ src/boot/EFI/ src/common/)

target_compile_options(
        SRC_BOOTLOADER

        PRIVATE
        -target x86_64-unknown-windows
        -ffreestanding
        -fshort-wchar
        -mno-red-zone
        -fno-builtin-printf
        -fno-builtin-free
        -Wno-fortify-source
        -O3
        
        )

target_link_options(
        SRC_BOOTLOADER

        PRIVATE
        -target x86_64-unknown-windows
        -nostdlib
        -Wl,-entry:EfiMain
        -Wl,-subsystem:efi_application
        -fuse-ld=lld-link
        )

#KERNEL

file(GLOB_RECURSE KERNEL_FILES CONFIGURE_DEPENDS src/kernel/*.c src/kernel/*.h src/kernel/*.asm src/kernel/*.cpp src/kernel/*.hpp src/common/*)

add_executable(SRC_KERNEL ${KERNEL_FILES})

set_target_properties(SRC_KERNEL PROPERTIES OUTPUT_NAME ${KERNEL_OUTPUT_NAME}.elf)

target_include_directories(SRC_KERNEL PRIVATE src/kernel src/common src/boot/EFI src/boot/bootloader/)

target_compile_options(
        SRC_KERNEL

        PRIVATE
        -Wall
        -fno-stack-protector
        -fPIC
        -mabi=sysv
        -mno-80387
        -mno-mmx
        -mgeneral-regs-only
        -mno-3dnow
        -mno-sse
        -mno-sse2
        -mno-red-zone
        -mcmodel=kernel
        -fno-builtin-printf
        -gdwarf-5
        -fno-omit-frame-pointer
        -fno-builtin-free
        -Wno-fortify-source
)

target_link_options(
        SRC_KERNEL

        PRIVATE
        -fPIE
        -nostdlib
        -static
        -static-pie
        -z max-page-size=0x1000
        -T ${PROJECT_SOURCE_DIR}/src/kernel/linker/kernel.ld
)

if(NOT EXISTS "bin/OVMF.fd")
    execute_process(COMMAND mkdir "bin/")
    #if you want it to clone the first time
    execute_process(COMMAND curl -Lo bin/OVMF.fd https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd)
    unset("bin/OVMF.fd" CACHE)
endif()

add_custom_target(
        create_image

        ALL
        COMMAND dd if=/dev/zero of=bin/${IMAGE_OUTPUT_NAME}.img bs=50K count=1 status=progress
        COMMAND mformat -i bin/${IMAGE_OUTPUT_NAME}.img ::
        COMMAND mmd -i bin/${IMAGE_OUTPUT_NAME}.img ::/EFI
        COMMAND mmd -i bin/${IMAGE_OUTPUT_NAME}.img ::/EFI/BOOT
        COMMAND mcopy -i bin/${IMAGE_OUTPUT_NAME}.img bin/BOOTX64.EFI ::/EFI/BOOT
        COMMAND mcopy -i bin/${IMAGE_OUTPUT_NAME}.img bin/${KERNEL_OUTPUT_NAME}.elf ::/EFI/BOOT
        COMMAND mcopy -i bin/${IMAGE_OUTPUT_NAME}.img ../src/boot/startup.nsh ::/

        USES_TERMINAL
        VERBATIM
)

#QEMU

set(
        DEFAULT_QEMU_FLAGS
        -m 4G
        -bios bin/OVMF.fd
        -drive file=bin/os.img
        -device VGA,edid=on,xres=1920,yres=1080
        -d int,guest_errors,cpu_reset
        -no-reboot
        -no-shutdown
        -serial stdio
)

add_custom_target(
        run

        COMMAND

        qemu-system-x86_64
        ${DEFAULT_QEMU_FLAGS}
        
        VERBATIM
        USES_TERMINAL
)

add_custom_target(
        debug

        COMMAND
        qemu-system-x86_64
        ${DEFAULT_QEMU_FLAGS}
        -S -s
        USES_TERMINAL
)
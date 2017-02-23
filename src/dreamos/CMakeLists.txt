# ------------------------------------------------------------------------------
# Initialize the project.
project(DreamOs C ASM)

# ------------------------------------------------------------------------------
# Set the project name.
set(PROJECT_NAME DreamOs)

# ------------------------------------------------------------------------------
# Add the BOCHS debug option.
option(ENABLE_BOCHS_DEBUG "Enable the bosh debug" ON)

# ------------------------------------------------------------------------------
# Add the memory option.
set(MEMORY "LEGACY" CACHE STRING "Type of memory")

# ------------------------------------------------------------------------------
# Set the compiler options.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdinc")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fomit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-builtin")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=i686")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D${MEMORY}")
if (ENABLE_BOCHS_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DBOCHS_DEBUG")
endif (ENABLE_BOCHS_DEBUG)

if(CMAKE_BUILD_TYPE MATCHES "Release")
    message(STATUS "Enablig RELEASE mode...")
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "Enablig DEBUG mode...")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")
endif(CMAKE_BUILD_TYPE MATCHES "Release")

# ------------------------------------------------------------------------------
# Set the assembly flags.
enable_language(ASM_NASM)
set(CMAKE_ASM_COMPILER nasm)
set(CMAKE_ASM_FLAGS "-f elf32 -g -F stabs")
set(CMAKE_ASM_COMPILE_OBJECT
        "<CMAKE_ASM_COMPILER> <FLAGS> -o <OBJECT> <SOURCE>")

# ------------------------------------------------------------------------------
# Add the includes.
include_directories(
        ..
        inc
        inc/drivers
        inc/fs
        inc/hardware
        inc/io
        inc/libc
        inc/lng
        inc/misc
        inc/shell
        inc/sys
        inc/system
        src/utils/include
)

# ------------------------------------------------------------------------------
# Add the source files.
set(SOURCE_FILES
        src/drivers/fdc.c
        src/drivers/keyboard.c
        src/drivers/mouse.c
        src/fs/fcntl.c
        src/fs/initrd.c
        src/fs/unistd.c
        src/fs/vfs.c
        src/hardware/timer.c
        src/hardware/cpuid.c
        src/hardware/pic8259.c
        src/io/io.c
        src/io/video.c
        src/libc/ctype.c
        src/libc/stdio.c
        src/libc/string.c
        src/libc/vsprintf.c
        src/misc/bitops.c
        src/misc/clock.c
        src/misc/debug.c
        src/shell/commands.c
        src/shell/shell.c
        src/shell/testing.c
        src/shell/user_shell.c
        src/sys/dirent.c
        src/sys/stat.c
        src/sys/utsname.c
        src/system/elf.c
        src/system/gdt.c
        src/system/handlers.c
        src/system/idt.c
        src/system/kheap.c
        src/system/paging.c
        src/system/panic.c
        src/system/scheduler.c
        src/system/syscall.c
        src/system/thread.c
        src/system/vm.c
        src/kernel.c
        src/system/thread_asm.s)

# ------------------------------------------------------------------------------
# Add the executable.
add_library(DreamOs ${SOURCE_FILES})

add_custom_command(
        TARGET DreamOs
        POST_BUILD
        COMMAND nasm
        -f elf ${CMAKE_CURRENT_SOURCE_DIR}/multicatcher.s
        -o ${CMAKE_CURRENT_BINARY_DIR}/bl.img
)

add_custom_command(
        TARGET DreamOs
        POST_BUILD
        COMMAND $(LD) -melf_i386
        -static
        --oformat elf32-i386
        --output=${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
        --script=${CMAKE_CURRENT_SOURCE_DIR}/kernel.lds
        ${CMAKE_CURRENT_BINARY_DIR}/bl.img
        ${CMAKE_CURRENT_BINARY_DIR}/libDreamOs.a
        -Ttext 0x100000
        -Map ${CMAKE_CURRENT_BINARY_DIR}/kernel.map
)

add_custom_command(
        TARGET DreamOs
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -E copy ${CMAKE_CURRENT_BINARY_DIR}/kernel.bin
        ${CMAKE_CURRENT_BINARY_DIR}/dreamos.img
)

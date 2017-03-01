# ------------------------------------------------------------------------------
# Initialize the project.
project(DreamOs C ASM)

# ------------------------------------------------------------------------------
# Set the project name.
set(PROJECT_NAME DreamOs)

# ------------------------------------------------------------------------------
# Add the BOCHS debug option.
option(ENABLE_BOCHS_DEBUG   "Enable the bosh debug" OFF)
option(ENABLE_QEMU_DEBUG    "Enable the qemu debug" OFF)

# ------------------------------------------------------------------------------
# Add the memory option.
set(MEMORY "LEGACY" CACHE STRING "Type of memory")

# ------------------------------------------------------------------------------
# Set the compiler options (original).
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
if (ENABLE_QEMU_DEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DQEMU_DEBUG")
endif (ENABLE_QEMU_DEBUG)

# ------------------------------------------------------------------------------
# Set the compiler options (extra).
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wconversion")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-qual")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdisabled-optimization")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfloat-equal")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfloat-conversion")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat=2")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-nonliteral")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-security")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-y2k")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wimport")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winit-self")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winline")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winvalid-pch")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-long-long")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-field-initializers")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-format-attribute")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmissing-include-dirs")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpacked")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpointer-arith")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wredundant-decls")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshadow")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstack-protector")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-aliasing=2")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch-default")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wswitch-enum")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunreachable-code")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-function")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-label")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-parameter")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-value")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-variable")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wvariadic-macros")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wwrite-strings")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-compare")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsign-conversion")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wuninitialized")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")

# ------------------------------------------------------------------------------

if (CMAKE_BUILD_TYPE MATCHES "Release")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")
endif (CMAKE_BUILD_TYPE MATCHES "Release")

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
        src/libc/queue.c
        src/libc/stdlib.c
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
        src/system/descriptor_tables.c
        src/system/elf.c
        src/system/gdt.c
        src/system/gdt.s
        src/system/isr.c
        src/system/isr.s
        src/system/idt.c
        src/system/idt.s
        src/system/kheap.c
        src/system/paging.c
        src/system/panic.c
        src/system/scheduler.c
        src/system/syscall.c
        src/system/thread.c
        src/system/vm.c
        src/system/mutex.c
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

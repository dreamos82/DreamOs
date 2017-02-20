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
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fs
        ${CMAKE_CURRENT_SOURCE_DIR}/include/hardware
        ${CMAKE_CURRENT_SOURCE_DIR}/include/io
        ${CMAKE_CURRENT_SOURCE_DIR}/include/libc
        ${CMAKE_CURRENT_SOURCE_DIR}/include/lng
        ${CMAKE_CURRENT_SOURCE_DIR}/include/misc
        ${CMAKE_CURRENT_SOURCE_DIR}/include/shell
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sys
        ${CMAKE_CURRENT_SOURCE_DIR}/include/system
        utils/include
)

# ------------------------------------------------------------------------------
# Add the source files.
set(SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/fdc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/keyboard.c
        ${CMAKE_CURRENT_SOURCE_DIR}/drivers/mouse.c
        ${CMAKE_CURRENT_SOURCE_DIR}/fs/fcntl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/fs/initrd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/fs/unistd.c
        ${CMAKE_CURRENT_SOURCE_DIR}/fs/vfs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hardware/8253.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hardware/cpuid.c
        ${CMAKE_CURRENT_SOURCE_DIR}/hardware/pic8259.c
        ${CMAKE_CURRENT_SOURCE_DIR}/io/io.c
        ${CMAKE_CURRENT_SOURCE_DIR}/io/video.c
        ${CMAKE_CURRENT_SOURCE_DIR}/libc/ctype.c
        ${CMAKE_CURRENT_SOURCE_DIR}/libc/stdio.c
        ${CMAKE_CURRENT_SOURCE_DIR}/libc/string.c
        ${CMAKE_CURRENT_SOURCE_DIR}/libc/vsprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/bitops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/misc/debug.c
        ${CMAKE_CURRENT_SOURCE_DIR}/shell/commands.c
        ${CMAKE_CURRENT_SOURCE_DIR}/shell/shell.c
        ${CMAKE_CURRENT_SOURCE_DIR}/shell/testing.c
        ${CMAKE_CURRENT_SOURCE_DIR}/shell/user_shell.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sys/dirent.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sys/stat.c
        ${CMAKE_CURRENT_SOURCE_DIR}/sys/utsname.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/elf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/gdt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/handlers.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/idt.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/kheap.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/paging.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/panic.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/scheduler.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/syscall.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/thread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/vm.c
        ${CMAKE_CURRENT_SOURCE_DIR}/kernel.c
        ${CMAKE_CURRENT_SOURCE_DIR}/system/thread_asm.s)

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

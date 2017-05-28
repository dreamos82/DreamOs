; kernel start location
; multiboot header
; author : shainer

; Grub is informed with this flag to load
; the kernel and kernel modules on a page boundary.
MBOOT_PAGE_ALIGN    equ 1<<0

; Grub is informed with this flag to provide the kernel
; with memory information.
MBOOT_MEM_INFO      equ 1<<1

; This is the multiboot magic value.
MBOOT_HEADER_MAGIC  equ 0x1BADB002

; This is the flag combination that we prepare for Grub
; to read at kernel load time.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

; Grub reads this value to make sure it loads a kernel
; and not just garbage.
MBOOT_CHECKSUM equ - (MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]       ; All instructions should be 32-bit.
[GLOBAL mboot]  ; Make 'mboot' accessible from C.
[EXTERN code]   ; Start of the '.text' section.
[EXTERN bss]    ; Start of the .bss section.
[EXTERN end]    ; End of the last loadable section.


mboot:
    dd MBOOT_HEADER_MAGIC   ; GRUB will search for this value on each
                            ; 4-byte boundary in your kernel file
    dd MBOOT_HEADER_FLAGS   ; Instructions for GRUB.
    dd MBOOT_CHECKSUM       ; Error check value for GRUB.
    dd mboot                ; This descriptor
    dd code                 ; Kernel '.text' (code) section.
    dd bss                  ; Kernel '.data' section
    dd end                  ; Kernel end.
    dd start                ; Kernel entry point.

[GLOBAL start]
[EXTERN kmain]              ; The start point of our C code

start:
  ;push esp                  ; initial stack
  push ebx                  ; push multiboot information
  cli                       ; execute the kernel
  call kmain
  jmp $                     ; Enter infinite loop to 'stop' the processor.

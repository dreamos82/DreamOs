; ***************************************************
; * * * * *          MultiCatcher           * * * * *
; * Author: shainer                                 *
; * Description: the Multiboot header               *
; ***************************************************


; Multiboot declarations
MBOOT_PAGE_ALIGN    equ 1	   ; Page aligning
MBOOT_MEM_INFO      equ 1<<1       ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic Number
; NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
; pass us a symbol table.
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]

[GLOBAL mboot]
[EXTERN code]                   ; Start of the '.text' section.
[EXTERN bss]                    ; Start of the .bss section.
[EXTERN end]                    ; End of the last loadable section.

mboot:
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
  dd  MBOOT_CHECKSUM            ; Check the other values
   
  dd  mboot
  dd  code                    ; Start of kernel '.text' (code) section.
  dd  bss                     ; End of kernel '.data' section.
  dd  end                     ; End of kernel.
  dd  start                     ; Kernel start point

[GLOBAL start]
[EXTERN main_loop]                   ; The start point of our C code

start:
  push    ebx                   ; Load multiboot header

; Real execution
  cli                         ; Disable interrupts.  x11-pc-it.map 
  call main_loop                   ; main() procedure, located in kernel.c
  jmp $                       ; Enter an infinite loop to "freeze" the OS after the boot
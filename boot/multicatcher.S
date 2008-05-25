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

mboot:
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS        ; How GRUB should load your file / settings
  dd  MBOOT_CHECKSUM            ; Check the other values
   
  dd  mboot
  dd  start                     ; Kernel start point

[GLOBAL start]
[EXTERN _start]                   ; The start point of our C code

start:
  push    ebx                   ; Load multiboot header

; Real execution
  cli                         ; Disable interrupts.  x11-pc-it.map 
  call _start                   ; main() procedure, located in kernel.c
  jmp $                       ; Enter an infinite loop to "freeze" the OS after the boot
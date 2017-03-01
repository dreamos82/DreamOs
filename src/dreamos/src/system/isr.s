[EXTERN IntTable]

[GLOBAL add_exception]

%macro restore_ec 0
    popl edx;
    popl ecx;
    popl eax;
%endmacro

%macro EXCEPTION 1
    global INT_%1
    INT_%1:
        cli                         ; Disable interrupts firstly.
        push byte 0                 ; Push a dummy error code.
        push %1                     ; Push the interrupt number.

        pusha                       ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

        call [IntTable + (4 * %1)]  ; Call the interrupt

        popa                        ; Pops edi,esi,ebp...
        add esp, 8                  ; Cleans up the pushed error code and pushed
                                    ;  ISR number.
        sti                         ; Enable interrupts.
        iret                        ; pops 5 things at once:
                                    ;   CS, EIP, EFLAGS, SS, and ESP
%endmacro

; This macro creates a stub for an ISR which passes it's own
; error code.
%macro EXCEPTION_EC 1
    global INT_%1
    INT_%1:
        cli                         ; Disable interrupts firstly.
        push byte 0                 ; Push a dummy error code.
        push %1                     ; Push the interrupt number.

        nop;
        xchgl eax, esp
        push ecx
        push edx

        mov edx,    0x10;
        mov dx,    ds;
        mov dx,    es;
        mov dx,    fs;

        push eax

%endmacro

; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
  global irq%1
  irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro

EXCEPTION 0
EXCEPTION 1
EXCEPTION 2
EXCEPTION 3
EXCEPTION 4
EXCEPTION 5
EXCEPTION 6
EXCEPTION 7

EXCEPTION 9

EXCEPTION 15
EXCEPTION 16

EXCEPTION 18
EXCEPTION 19
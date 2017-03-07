[EXTERN IntTable]
[EXTERN interrupt_handler]
[EXTERN syscall_handler]

%macro EXCEPTION 1
    [GLOBAL INT_%1]
    INT_%1:
        pusha                       ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
        push %1                     ; Push the interrupt number.
        call [IntTable + (4 * %1)]  ; Call the interrupt
        add esp, 4                  ; Cleans up the pushed ISR number.
        popa                        ; Pops edi,esi,ebp...
        iret                        ; pops 5 things at once:
                                    ;   CS, EIP, EFLAGS, SS, and ESP
%endmacro

; This macro creates a stub for an ISR which passes it's own
; error code.
%macro EXCEPTION_EC 1
    [GLOBAL INT_%1]
    INT_%1:
        nop
        xchg    [esp], eax
        push    ecx
        push    edx
        mov     edx, 0x10
        mov     ds, dx
        mov     es, dx
        mov     fs, dx
        push    eax
        push    %1                     ; Push the interrupt number.
        call    [IntTable + (4 * %1)]  ; Call the interrupt
        add     esp, 8
        pop     edx
        pop     ecx
        pop     eax
        iret                        ; pops 5 things at once:
                                    ;   CS, EIP, EFLAGS, SS, and ESP
%endmacro

%macro IRQ 1
    [GLOBAL INT_%1]
    INT_%1:
        pusha                       ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
        mov     eax, esp;
        push    eax;
        call    interrupt_handler   ; Call the interrupt handler.
        pop     eax;
        mov     esp, eax;
        popa                        ; Pops edi,esi,ebp...
        iret                        ; pops 5 things at once:
                                    ;   CS, EIP, EFLAGS, SS, and ESP
%endmacro

%macro SYSCALL 1
    [GLOBAL INT_%1]
    INT_%1:
        pusha                       ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
        call syscall_handler        ; Call the interrupt
        popa                        ; Pops edi,esi,ebp...
        iret                        ; pops 5 things at once:
                                    ;   CS, EIP, EFLAGS, SS, and ESP
%endmacro

; Initialize the exceptions.
EXCEPTION 0
EXCEPTION 1
EXCEPTION 2
EXCEPTION 3
EXCEPTION 4
EXCEPTION 5
EXCEPTION 6
EXCEPTION 7
EXCEPTION_EC 8
EXCEPTION 9
EXCEPTION_EC 10
EXCEPTION_EC 11
EXCEPTION_EC 12
EXCEPTION_EC 13
EXCEPTION_EC 14
EXCEPTION 15
EXCEPTION 16
EXCEPTION_EC 17
EXCEPTION 18
EXCEPTION 19

; Initialize the system calls.
SYSCALL 80

IRQ 32
IRQ 33
IRQ 34
IRQ 35
IRQ 36
IRQ 37
IRQ 38
IRQ 39
IRQ 40
IRQ 41
IRQ 42
IRQ 43
IRQ 44
IRQ 45
IRQ 46
IRQ 47
IRQ 48

[EXTERN IntTable]
[EXTERN _irqinterrupt]
[EXTERN syscall_handler]

%macro EXCEPTION 1
    [GLOBAL INT_%1]
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
    [GLOBAL INT_%1]
    INT_%1:
        cli                         ; Disable interrupts firstly.
        push byte 0                 ; Push a dummy error code.
        push %1                     ; Push the interrupt number.

        nop
        xchg eax, esp
        push ecx
        push edx

        mov edx,    0x10;
        mov dx,     ds;
        mov dx,     es;
        mov dx,     fs;

        push eax

        call [IntTable + (4 * %1)]  ; Call the interrupt

        add esp, 8                  ; Cleans up the pushed error code and pushed
                                    ;  ISR number.

        pop edx;
        pop ecx;
        pop eax;

        sti                         ; Enable interrupts.
        iret                        ; pops 5 things at once:
                                    ;   CS, EIP, EFLAGS, SS, and ESP

%endmacro

%macro IRQ 1
    [GLOBAL INT_%1]
    INT_%1:
        cli                         ; Disable interrupts firstly.
        push byte 0                 ; Push a dummy error code.
        push %1                     ; Push the interrupt number.
        pusha                       ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

        ;mov     esp, eax;
        ;push    eax;
        call    _irqinterrupt       ; Call the interrupt
        ;pop     eax;
        ;mov     eax, esp;

        popa                        ; Pops edi,esi,ebp...
        add esp, 8                  ; Cleans up the pushed error code and pushed
                                    ;  ISR number.
        sti                         ; Enable interrupts.
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

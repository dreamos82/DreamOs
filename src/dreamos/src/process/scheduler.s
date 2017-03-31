[global switch_process]

switch_process:
    ; --------------------------------------------------------------------------
    ; Take the first argument.
    mov eax, [esp +  4]         ; (process_t ** current)
    mov eax, [eax     ]         ; (process_t *  current)
    mov eax, [eax     ]         ; (process_t   current)

    ; --------------------------------------------------------------------------
    ; Copy the values of the current registers inside the current_process
    mov [eax +  0], esp
    mov [eax +  4], ebp
    mov [eax +  8], ebx
    mov [eax + 12], esi
    mov [eax + 16], edi

    ; --------------------------------------------------------------------------
    ; Deal with the status of the processor.
    pushf                       ; Copy FLAGS register to the new top of stack.
    pop ecx                     ; Put the value of the FLAGS register on ecx.
    mov [eax + 20], ecx         ; Store the flag values inside eflags.

    ; --------------------------------------------------------------------------
    ; Take the second argument.
    mov eax, [esp +  8]         ; EAX = (process_t * next)
    ; Change the pointer of current_process to next.
    mov edx, [esp +  4]         ; EDX = (process_t ** current)
    ; Now copy the pointer to the next process inside the pointer to the current.
    mov [edx], eax              ; C equivalent: (*current) = next;
    ; Deference again so that we have inside EAX the structu of the next process.
    mov eax, [eax     ]         ; (process_t next)

    ; --------------------------------------------------------------------------
    ; Copy the values of the current registers inside the current_process
    mov esp, [eax +  0]
    mov ebp, [eax +  4]
    mov ebx, [eax +  8]
    mov esi, [eax + 12]
    mov edi, [eax + 16]

    ; --------------------------------------------------------------------------
    ; Deal with the status of the processor.
    mov eax, [eax + 20]         ; Copy the eflag variable value inside EAX.
    push eax                    ; Push EAX on top of stack.
    popf                        ; Pops eflag value inside flags register.

    ret

    [global switch_thread]
    [global _create_thread]
    [extern current_thread]
    [extern thread_exit]

switch_thread:
    ; Get the current thread.
    mov eax, [current_thread]
    mov eax, [eax]

    ; Get the parameters of the thread.
    mov [eax+0],  esp
    mov [eax+4],  ebp
    mov [eax+8],  ebx
    mov [eax+12], esi
    mov [eax+16], edi

    ; Decrements the SP by 2 and copies the FLAGS register to the new top of
    ;  stack.
    pushf
    ; Put the value of the FLAGS register on ecx.
    pop ecx
    ; Then move the value of the FLAGS register inside the eflags variable
    ;  of the thread.
    mov [eax+20], ecx

    ; Move inside eax the second element inside the stack of the thread,
    ;  which is the 'thread_exit' function.
    mov eax, [esp+4]

    mov [current_thread], eax
    mov eax, [eax]

    mov esp, [eax+0]
    mov ebp, [eax+4]
    mov ebx, [eax+8]
    mov esi, [eax+12]
    mov edi, [eax+16]

    mov eax, [eax+20]
    ; Push the thread on the stack.
    push eax
    ; Pops the word from the top of the stack and stores the value in the
    ;  flags register.
    popf

    ret
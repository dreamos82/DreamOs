    [global switch_thread]
    [global switch_thread_2]

    [extern current_thread]
    [extern printf]
    [extern thread_exit]

switch_thread:
    ; --------------------------------------------------------------------------
    ; Get the current thread.
    mov eax, [current_thread]   ; Move in eax pointer to thread.
    mov eax, [eax]              ; Move thread content to eax.

    pushf                       ; Copy FLAGS register to the new top of stack.
    pop ecx                     ; Put the value of the FLAGS register on ecx.

    ; --------------------------------------------------------------------------
    ; Copy the values of the current registers inside the current_thread
    mov [eax +  0], esp
    mov [eax +  4], ebp
    mov [eax +  8], ebx
    mov [eax + 12], esi
    mov [eax + 16], edi
    mov [eax + 20], ecx

    ; --------------------------------------------------------------------------
    ; Move inside EAX the second element inside the stack of the thread,
    ;  which is the 'thread_exit' function.
    mov eax, [esp +  4]
    mov [current_thread], eax
    mov eax, [eax     ]

    ; --------------------------------------------------------------------------
    ; Copy the values of the current registers inside the current_thread
    mov esp, [eax +  0]
    mov ebp, [eax +  4]
    mov ebx, [eax +  8]
    mov esi, [eax + 12]
    mov edi, [eax + 16]
    mov eax, [eax + 20]

    push eax            ; Push the thread on the stack.
    popf                ; Pops from top of stack and stores in flags register.

    ret

switch_thread_2:
    push    ebp             ; create stack frame
    mov     ebp, esp
    mov     eax, [ebp +  8] ; grab the  first argument (current_thread)
    mov     edx, [ebp + 12] ; grab the second argument (new_thread)



    pop     ebp             ; restore the base pointer
    ret
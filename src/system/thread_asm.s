        [global switch_thread]
        [global _create_thread]
        [extern running_thread] ;This is a thread_list_t *
        [extern current_thread] ;This is a thread_t *
        [extern thread_exit]
        
switch_thread:
        mov eax, [running_thread]
        mov eax, [eax]

        mov [eax+0],  esp
        mov [eax+4],  ebp
        mov [eax+8],  ebx
        mov [eax+12], esi
        mov [eax+16], edi
        
        pushf
        pop ecx
        mov [eax+20], ecx

        mov eax, [esp+4]

        mov [running_thread], eax
        mov eax, [eax]
        
        mov esp, [eax+0]
        mov ebp, [eax+4]
        mov ebx, [eax+8]
        mov esi, [eax+12]
        mov edi, [eax+16]
        
        mov eax, [eax+20]
        push eax
        popf
        
        ret

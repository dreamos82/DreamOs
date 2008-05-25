;Dream Catcher
;Bootloader per Dream O.S.
;Versione: 0.1prealpha
;Questo bootloader e' in grado di caricare massimo un kernel di
;dimensione minore di circa 10 kib e' ancora una versione di test.
;Siamo riusciti a risolvere il problema del caricamento del bootloader.
;ToDo: Pulire schermo prima di fare qualsiasi operazione di stampa

[BITS 16]
[ORG 0x7C00]

xor ax, ax
mov ds, ax
mov ss, ax
mov sp, 0xFFFE                        ;Imposto lo Stack

mov bp, CLEAR
mov cx, 1
mov dh, 1
mov dh, -1
mov dl, -1
clear_v:
inc dh
clear_h:
inc dl
call Stampastringa
cmp dl, 80
jne clear_h
cmp dh, 25
jne clear_v

mov bp, WELCOM
mov dh, 0
mov dl, 0
mov cx, 26
call Stampastringa                   ;Un Semplice messaggio di benvenuto

mov bp, WELCOM1
mov dh, 1                              ;imposto la riga del messaggio
mov dl, 0                               ;imposto la colonna di partenza del messaggio
mov cx, 22
call Stampastringa                   ;Un Semplice messaggio di benvenuto continua

mov bp, LEGGIKERNEL
mov dh, 2
mov dl, 0
mov cx, 18
call Stampastringa
call Posiziona                       ;Mi posiziono col cursore, nella 4a riga partendo dall'alto
call carica_kernel                   ;carico il kernel
call spegni_floppy

mov bp, KERNELDONE
mov dh, 3
mov dl, 0
mov cx, 22
call Stampastringa

caricapmode:
	mov bp, PMODELOAD
	mov dh, 4
	mov dl, 0
	mov cx, 17
	call Stampastringa
	cli			;disabilito gli interrupt

waitfree1:
	in al, 0x64		;metto in ax l'indirizzo della linea da leggere
	test al, 2
	jnz waitfree1
		mov al, 0xD1
		out 0x64, al
waitfree2:
	in al, 0x64	
	and ax, byte 2
	jnz waitfree2
	mov al, 0xDF
	out 0x60, al

lgdt[info_gdt]		;carico la gdt

mov eax,cr0   ;  metto in eax cr0
or  al,1      ;  imposto bit per pmode
mov cr0,eax   ;  entro in modalita protetta

jmp dword (gdt_code-gdt_table):pmode	; tutto e' pronto, e quindi eseguo il codice a 32 bit

[BITS 32]
pmode:
;sto indicando che si tratta di codice a 32bit

	mov  ax,gdt_data-gdt_table	;metto 0x10 in ax
        mov ds, ax    	;imposto ds
        mov es, ax    	;imposto es
        mov fs, ax     	;imposto fs
        mov   gs, ax  	;imposto gs
	mov   ss, ax  	;imposto ss

	mov esp,  0x9FFFC     ;imposto lo stack
	
        push dword 2  ;azzero gli EFLAG
        popfd   ;e li imposto

       mov eax, 0x10000        ;la locazione di memoria dove ci sta kernel

       jmp eax        ;lancio il kernel!!!
       
       cli
       hlt

;;;;;;;;;;;;;;;;;;;SEZIONE PROCEDURE E FUNZIONI;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]
Stampastringa:
	;parametri di default per la stampa di una stringa
	;posizione,  num caratteri e stringa da mettere saranno 
	;passati al programma principale
	mov ah, 13h			;Servizio stampa stringa
	mov al, 0
	xor bx, bx
	mov bh, 0
	mov bl, 7h      		;la nostra scritta sar?bianca su sfondo nero
	int 10h
	ret

Posiziona:
	mov ah, 02h			;servizio per posizionare il cursore
	mov bh, 0
	mov dh, 4
	mov dl, 0
	int 10h
	ret

StampaUguale:
	mov ah, 0eh			;Servizio di stampa carattere
	mov al, '='			;Devo stampare uguale
	mov bh, 0
	mov bl, 0
	push dx
	mov dh, 4
	mov dl, 0
	int 10h				;Faccio partire la stampa
	pop dx
	mov ah, 2
	mov al, 1
	ret

spegni_floppy:
  mov dx,0x3f2
  mov al,0x0c
  out dx,al
  ret

carica_kernel:
	;Carica il kernel all'indirizzo 0x1000:0
	;INPUT:
	;di = dimensione del kernel in settori
	;OUTPUT:  nessuno
		mov di, 450         ;Dimensione del kernel in settori
		mov ax, 0x1000  ;copio 0x1000 in ax
		mov es, ax      ; e quindi imposto il segmento

		mov ch, 0       ;imposto il numero della traccia
		mov cl, 2       ;imposto il numero del settore

		mov dh, 0       ;numero della testina
		mov dl, 0       ;numero del drive
		
		xor bx, bx      ;metto a zero il buffer         es:bx  = 0x1000:0

	leggi:
                mov ah, 2       ;imposto il numero del servizio
		mov al, 1       ;indico quanti settori leggere
	
		int 0x13	;ora leggo davvero
		call StampaUguale
		jc fdc_error	;se c'?stato un errore salto alla gestione dell'errore

	        push bx         ;salvo il valore di bx  
		pop bx          ;ripristiamo il valore di bx

		mov ax,es       ;metto il valore di es in ax
		add ax,32       ;quindi modifico questo valore
		mov es,ax       ;e riaggiorno es

		dec di          ;decrementiamo di uno il numero dei settori
		jz _fine ;abbiamo finito

		
		inc cx  ;incremento il numero del settore
		cmp cl,18	;comparo cl e 18
		jbe leggi       ;se ?< 18 allora posso continuare a leggere
		mov cl,1        ;altrimenti riinizio dal settore 1
		inc dh  ;incremento il numero della testina
		cmp dh,2	;comparo dh e 2
		jne leggi       ;se ?la testina 1 allora tutto bene
		mov dh,0       ;altrimenti imposto la testina 0
		inc ch  ;e incremento il numero della traccia
		jmp leggi	;ora tutto ?apposto posso continuare a leggere

	fdc_error:	;gestione dell'errore
		mov bx, KERNELERRO	;metto in bx l'indirizzo del messaggio di errore
		call Stampastringa  	; e lo stampo
	_fine:	;fine della routine
	        ret  ;bene abbiamo finito

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;SEZIONE GDT;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

info_gdt:
	dw lunghezza
	dw gdt_table

gdt_table:

	gdt_null_desc:
		dd 0
		dd 0
	gdt_code:
		dw 0xFFFF
		dw 0x0000
		db 0x00
		db 0x9A
		db 0xCF
		db 0x00
	
	gdt_data:
		dw 0xFFFF
		dw 0x0000
		db 0x00
		db 0x92
		db 0xCF
		db 0x00
	
lunghezza equ $ - gdt_table

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;SEZIONE DATI;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WELCOM	db  'Benvenuto in Dreamcatcher '        ;dichiariamo la stringa di benvenuto
WELCOM1 db  'bootloader per dreamos'
LEGGIKERNEL db 'Caricamento Kernel'
KERNELDONE db 'Caricamento Completato'
KERNELERRO db 'Errore Lettura'
PMODELOAD db 'Caricamento pmode'
CLEAR	db  ' '

times 510-($-$$) db 0           ;riempiamo tutto lo spazio fino a 510byte di 0
dw 0xAA55       ;il nostro carattere terminatore

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;    DreamCatcher Version 0.2 Fat Support         ;;
;;                                                 ;;
;;    Versione del bootloader Riscritta da 0       ;;
;;    Con supporto per il boot da file system      ;;
;;    Fat.                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

[BITS 16]
[ORG 0x7C00]
jmp short START
nop

OEM_IDENTIFIER    db "mkdosfs "
NUMBER_BYTES      dw 0x0200   ;numero di byte per settore
SECTORS_UNIT      db 0x01
RESERVED_SECTORS  dw 0x0001
NUMBER_FATS       db 0x02
DIR_NUMBER        dw 0x00e0   ;numero di direcctory max
VOLUME_SECTORS    dw 0x0b40
MEDIA_TYPE        db 0xf0
NUMBER_SECTORS    dw 0x0009   ;numero di settori per fat
SECTORS_PTRACK    dw 0x0012
HEADS_DISK        dw 0x0002
SIGNATURE         db 0x29
VOLUME_ID         dd 0xFFFFFFFF
VOLUME_LABEL      db "DREAMOS    "
SYSTEM_ID         db "FAT12   "
DUE		  db 2
		
START:
	xor ax, ax
	mov es, ax
	mov ds, ax
	mov fs, ax
	mov gs, ax

	mov sp, 0xFFFF		; Imposta lo stack

	;; Carica la RootDir all'indirizzo 0x0200:0x0000
	call Calcola_dim_root
	call Trova_root_dir
	mov bx, 0x0200
	mov es, bx
	xor bx, bx
	call CaricaSettori

cerca_dreamos:
	mov si, DREAMOS
	mov di, bx
	mov cx, 11
	repe cmpsb		; ciclo per vedere se le stringhe sono uguali
	je dreamos_found
	add bx, 0x0020		; passa alla prossima entry
	cmp bx, 0x00e0
	je CICLO
	jmp cerca_dreamos

dreamos_found:
	push ds
	mov cx, es
	mov ds, cx
	
	mov di, bx
	mov dx, WORD [di+26] ; primo cluster del file in dx
	pop ds

	mov ax, 2
	mov cx, 18
	mov bx, 0x0200
	mov es, bx
	xor bx, bx
	push dx
	call CaricaSettori
	pop dx

	;; dx = 0x004e
leggi_cluster:
	;; call CaricaCluster
	mov bx, 0x0200
	add bx, dx
	mov cx, WORD [bx]
	cmp cx, 0x6035
	je corretto
	cmp cx, 0x0fff
	je corretto
	test dx, 0x0001
	jnz dispari
pari:
	and cx, 0xfff
	jmp ripeti
dispari:
	shr cx, 4
ripeti:
	mov ax, cx
	shr ax, 1
	add cx, ax
	mov dx, cx
	jmp leggi_cluster
	
corretto:
	mov si, CORRETTO
	mov di, 9
	call StampaStringa
CICLO:
	jmp CICLO

	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;						   ;;
;;       AREA FUNZIONI E PROCEDURE VARIE	   ;;
;;						   ;; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CaricaCluster:
	mov cx, 0x1000
	mov es, cx
	mov cx, 1
	mov ax, dx
	mov bx, WORD [Offset]
	call CaricaSettori
	add bx, 0x0200
	mov WORD [Offset], bx
	ret
		
;; StampaStringa - Stampa una stringa a video
;; Argomenti: di = dimensione stringa
;;            si = stringa da leggere
;; Ritorno  : Nessuno
StampaStringa:
     lodsb			; carica il prossimo carattere
     dec di
     jz      .DONE
     mov     ah, 0x0E		; servizio di stampa a video
     mov     bh, 0x00		; pagina 0
     mov     bl, 0x07		; attributi
     int     0x10               ; invoca il BIOS
     jmp     StampaStringa
.DONE:
     ret

;; Calcola_dim_root - Calcola il numero di settori occupato dalla RootDir
;; Argomenti: Nessuno
;; Ritorno  : cx = dimensione in settori
Calcola_dim_root:
    mov ax, 32		; numero di byte di una entry della dir
    mul word [DIR_NUMBER]
    div word [NUMBER_BYTES]    
    mov cx, ax
    ret

;; Trova_root_dir - Trova il settore di partenza della RootDir
;; Argomenti: Nessuno
;; Ritorno  : ax = settore di partenza
Trova_root_dir:
    xor ax, ax
    mov al,  byte [NUMBER_FATS]		; Metto il numero di fat dentro il registro al
    mul word [NUMBER_SECTORS]		; lo moltiplico per il numero di settori per fat
    add ax, word [RESERVED_SECTORS]	; gli aggiungo il numero di settori riservati
    mov word [Lba], ax
    ret

;; CaricaSettori - Carica settori in memoria
;; Argomenti: ax    = settore di partenza
;;	      cx    = numero di settori da caricare
;;	      es:bx = zona di memoria in cui caricare
;; Ritorno  : Nessuno 
CaricaSettori:	
    pusha			; salva tutti i registri generali sullo stack
    call Converti_CHS		; calcola i valori giusti per cilindro, settore e testina
    mov ah, 0x02		; servizio di caricamento
    mov al, 0x01
    mov ch, BYTE [Cilindro]
    mov cl, BYTE [Settore]
    mov dh, BYTE [Testina]
    mov dl, 0			; numero del drive
    int 0x13
    jc fdc_error		; se CF è settato ci sono stati errori

    popa
    dec cx
    jz fine_lettura
    inc ax			; passa al prossimo settore
    jmp CaricaSettori
    ret

fdc_error:
	mov si, ERROR
	mov di, 18
	call StampaStringa
	jmp CICLO

fine_lettura:
	ret

;; Converti_CHS - Converte da settore logico a Cilindro - Testina - Settore 
Converti_CHS:
    xor dx, dx
	
    ;; Appoggio = SECTORS_PTRACK * HEADS_DISK
    mov ax, [SECTORS_PTRACK]
    mul word [HEADS_DISK]
    mov word [Appoggio], ax

    ;; Cilindro = Lba / Appoggio
    mov ax, word [Lba]
    div word [Appoggio]
    mov byte [Cilindro], al

    ;; Appoggio = Lba % (Appoggio)
    xor dx, dx			; Azzero dx perchè l'operazione è a 16 bit
    mov ax, word [Lba]
    div word [Appoggio]
    mov cx, dx
	
    ;; Testina = cx / SPT 
    ;; Settore = cx % SPT + 1   
    xor dx, dx
    mov ax, cx
    div word [SECTORS_PTRACK]    
    inc dx  
    mov byte [Testina], al
    mov byte [Settore], dl
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;				      ;; 
;; Sezione variabili e strutture dati ;; 
;;				      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

WELCOM	  db	'DreamCatcher v0.2 BootLoader per Dreamos',13,10,0
CORRETTO  db	'Corretto', 13,10,0
DREAMOS   db	'DREAMOS IMG', 13,10,0
ERROR	  db	'Errore nel floppy' ,13,10,0
UGUALE	  db	'=' ,13,10,0
Lba	  dw	0x0000
Appoggio  db	0x00
Cilindro  db	0x00
Testina	  db	0x00
Settore	  db	0x00
Offset	  dw    0x0000
Cluster	  dw	0x0000
	
; Fine
times 510-($-$$) db 0		; Riempe il resto di 0
dw 0xAA55			; Aggiunge la firma del bootloader per marcare il device come avviabile
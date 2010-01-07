ARCH = x86


PROMPT = " -> " 

LD = @echo "   " $(PROMPT) LD "    " $ && ld
CC = @echo "   " $(PROMPT) CC "    " $< && gcc
ASM = @echo "   " $(PROMPT) ASM "   " $< && nasm
AR = @echo "   " $(PROMPT) AR "    " $ && ar
CD = @echo  $(PROMPT) CD "        " $(DIR) && cd


.c.o: 
	$(CC) -c $(CFLAGS) -o "$@" "$<"

.s.o:
	$(ASM) $(ASFLAGS) $<

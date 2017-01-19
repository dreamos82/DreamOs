ARCH = x86

PROMPT = " -> "
SRC = src

LD = @echo "   " $(PROMPT) LD "    " $ && ld
CC = @echo "   " $(PROMPT) CC "    " $< && gcc
ASM = @echo "   " $(PROMPT) ASM "   " $< && nasm
AR = @echo "   " $(PROMPT) AR "    " $ && ar
CD = @echo  $(PROMPT) CD "        " $(DIR) && cd

$(GENDIR)/%.o: $(SRC)/%.c
	mkdir -p "$(@D)"
	$(CC) -c $(CFLAGS) -o "$@" "$<"

%.o: %.s
	$(ASM) $(ASFLAGS) $<

ARCH = x86

PROMPT = " -> "
OUTPUT = generated
SRC = src

LD = @echo "   " $(PROMPT) LD "    " $ && ld
CC = @echo "   " $(PROMPT) CC "    " $< && gcc
ASM = @echo "   " $(PROMPT) ASM "   " $< && nasm
AR = @echo "   " $(PROMPT) AR "    " $ && ar
CD = @echo  $(PROMPT) CD "        " $(DIR) && cd

$(OUTPUT)/%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS) -o "$@" "$<"

%.o: %.s
	$(ASM) $(ASFLAGS) $<

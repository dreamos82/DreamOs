ARCH = x86

PROMPT = " -> "
SRC = src

ifeq ($(ISWINDOWS), false)
LD = echo "   " $(PROMPT) LD "    " $ && ld
CC = echo "   " $(PROMPT) CC "    " $< && gcc
AR = echo "   " $(PROMPT) AR "    " $ && ar
else
LD = echo "   " $(PROMPT) LD "    " $ && i686-elf-ld
CC = echo "   " $(PROMPT) CC "    " $< && i686-elf-gcc
AR = echo "   " $(PROMPT) AR "    " $ && i686-elf-ar
endif

ASM = echo "   " $(PROMPT) ASM "   " $< && nasm
CD = echo  $(PROMPT) CD "        " $(DIR) && cd

$(GENDIR)/%.o: $(SRC)/%.c
	mkdir -p "$(@D)"
	$(CC) -c $(CFLAGS) -o "$@" "$<"

%.o: %.s
	$(ASM) $(ASFLAGS) $<

CFLAGS = -nostdlib -fomit-frame-pointer -fno-builtin -Wall -march=i386 -I./include -I./include/io -I./include/drivers -I./include/libc -I./include/processore -I./include/hardware -I./include/mem -I./include/system
OBJ = kernel.o io/video.o drivers/keyboard.o libc/ctype.o libc/string.o io/io.o libc/stdio.o hardware/cpuid.o hardware/keyboard.o processore/gdt.o processore/idt.o processore/handlers.o hardware/pic8259.o mem/fismem.o mem/paging.o system/syscall.o hardware/8253.o

dreamos.img: bl.img kernel.bin
	mv kernel.bin dreamos.img

bl.img : boot/multicatcher.S
	nasm -f elf ./boot/multicatcher.S -o bl.img

kernel.bin: $(OBJ)
	ld -static --oformat elf32-i386 --output=kernel.bin --script=kernel.lds bl.img $(OBJ) -Ttext 0x100000 -Map kernel.map

kernel.o: kernel.c
io/video.o: io/video.c
io/io.o : io/io.c
processore/gdt.o: processore/gdt.c
processore/idt.o: processore/idt.c
processore/handlers.o: processore/handlers.c
hardware/pic8259.o: hardware/pic8259.c
hardware/cpuid.o: hardware/cpuid.c
hardware/keyboard.o: hardware/keyboard.c
libc/stdio.o : libc/stdio.c
libc/ctype.o : libc/ctype.c
libc/string.o : libc/string.c
mem/fismem.o : mem/fismem.c
mem/paging.o : mem/paging.c
drivers/keyboard.o : drivers/keyboard.c
system/syscall.o : system/syscall.c
hardware/8253.o : hardware/8253.c


.PHONY: clean install qemu

clean:
	rm -f *.img *.bin *.map
	rm -f $(OBJ)

install: dreamos.img
	dd if=dreamos.img of=/dev/fd0

qemu: dreamos.img
	dd if=/dev/zero bs=$(shell let bs=1474560 -$(shell stat --format=%s dreamos.img); echo $$bs) count=1 2>/dev/null | cat dreamos.img - > dreamos_padded.img

it:
	cp include/lng/it.h include/use.h

en:
	cp include/lng/en.h include/use.h

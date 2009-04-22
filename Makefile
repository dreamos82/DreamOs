# Please set your kernel preference versions..
# Enjoy your self :)
# Osiris at jeek69@katamail.com

NAME = DreamOS
VERSION = 0
PATCHLEVEL = 1.1
EXTRAVERSION = -trunk

CFLAGS = -nostdlib -nostdinc -fomit-frame-pointer -fno-builtin -fno-stack-protector -Wall -march=i686 -m32 -I./include -I./include/io -I./include/drivers -I./include/libc -I./include/processore -I./include/hardware -I./include/mem -I./include/system -I./include/shell -I./include/misc

OBJ = kernel.o\
      io/video.o\
      drivers/keyboard.o\
      libc/ctype.o\
      libc/string.o\
      io/io.o\
      libc/stdio.o\
      hardware/cpuid.o\
      hardware/keyboard.o\
      processore/gdt.o\
      processore/idt.o\
      processore/handlers.o\
      hardware/pic8259.o\
      mem/fismem.o\
      mem/paging.o\
      mem/kheap.o\
      misc/clock.o\
      misc/bitops.o\
      misc/ordered_list.o\
      misc/debug.o \
      system/syscall.o\
      hardware/8253.o\
      shell/shell.o\
      shell/commands.o\
      sys/utsname.o

dreamos.img: bl.img kernel.bin
	mv kernel.bin dreamos.img

bl.img : boot/multicatcher.S
	nasm -f elf ./boot/multicatcher.S -o bl.img

kernel.bin: $(OBJ)
	ld -static --oformat elf32-i386 --output=kernel.bin --script=kernel.lds bl.img $(OBJ) -Ttext 0x100000 -Map kernel.map

kernel.o: kernel.c
io/video.o: io/video.c
io/io.o: io/io.c
processore/gdt.o: processore/gdt.c
processore/idt.o: processore/idt.c
processore/handlers.o: processore/handlers.c
hardware/pic8259.o: hardware/pic8259.c
hardware/cpuid.o: hardware/cpuid.c
hardware/keyboard.o: hardware/keyboard.c
libc/stdio.o: libc/stdio.c
libc/ctype.o: libc/ctype.c
libc/string.o: libc/string.c
mem/fismem.o: mem/fismem.c
mem/paging.o: mem/paging.c
mem/kheap.o: mem/kheap.c
misc/clock.o: misc/clock.c
misc/ordered_list.o: misc/ordered_list.c
misc/bitops.o: misc/bitops.c
misc/debug.o: misc/debug.c
drivers/keyboard.o: drivers/keyboard.c
system/syscall.o: system/syscall.c
hardware/8253.o: hardware/8253.c
shell/shell.o: shell/shell.c
shell/commands.o: shell/commands.c
sys/utsname.o: sys/utsname.c

img:
	mount -o loop boot/grub.img boot/os
	cp dreamos.img boot/os/boot/grub/
	umount boot/os

vers: 
	 sed -i -e "/^#define VERSION/s/\".*\"/\"$(VERSION)\"/" include/version.h
	 sed -i -e "/^#define PATCHLEVEL/s/\".*\"/\"$(PATCHLEVEL)\"/" include/version.h
	 sed -i -e "/^#define EXTRAVERSION/s/\".*\"/\"$(EXTRAVERSION)\"/" include/version.h
	 sed -i -e "/^#define NAME/s/\".*\"/\"$(NAME)\"/" include/version.h
	 sed -i -e "/^#define REV_NUM/s/\".*\"/\""-r`cat .svn/entries | head -n 4 | tail -n 1`"\"/" include/version.h

.PHONY: clean install qemu

clean:
	rm -f *.img *.bin *.map
	rm -f $(OBJ)

install:
	mkfs.ext2 /dev/fd0
	dd if=boot/grub.img of=/dev/fd0

qemu: dreamos.img
	dd if=/dev/zero bs=$(shell let bs=1474560 -$(shell stat --format=%s dreamos.img); echo $$bs) count=1 2>/dev/null | cat dreamos.img - > dreamos_padded.img

it:
	cp include/lng/it.h include/use.h

en:
	cp include/lng/en.h include/use.h

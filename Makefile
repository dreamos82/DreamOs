# Please set your kernel preference versions..
# Enjoy your self :)
# Osiris at osiris@Devils.com

NAME = DreamOS
VERSION = 0
PATCHLEVEL = 3
EXTRAVERSION = -trunk
MEMORY = LEGACY
include Makefile.am

CFLAGS = -nostdlib\
	-nostdinc\
	-g\
	-fomit-frame-pointer\
	-fno-builtin\
	-fno-stack-protector\
	-Wall\
	-march=i686\
	-m32\
	-I./src/include\
	-I./src/include/io\
	-I./src/include/drivers\
	-I./src/include/libc\
	-I./src/include/cpu\
	-I./src/include/hardware\
	-I./src/include/mem\
	-I./src/include/system\
	-I./src/include/shell\
	-I./src/include/misc\
	-I./src/include/fs\
	-I./src/include/sys\
	-I./src/include/tasks\
	-I./src/include/tasks/tss\
	-DBOCHS_DEBUG\
	-D$(MEMORY)

OBJ = generated/kernel.o\
	generated/io/video.o\
	generated/drivers/keyboard.o\
	generated/drivers/mouse.o\
	generated/drivers/fdc.o\
	generated/fs/vfs.o\
	generated/fs/fcntl.o\
	generated/fs/initrd.o\
	generated/fs/unistd.o\
	generated/libc/ctype.o\
	generated/libc/string.o\
	generated/io/io.o\
	generated/libc/stdio.o\
	generated/hardware/cpuid.o\
	generated/cpu/gdt.o\
	generated/cpu/idt.o\
	generated/cpu/handlers.o\
	generated/hardware/pic8259.o\
	generated/mem/fismem.o\
	generated/mem/paging.o\
	generated/mem/kheap.o\
	generated/misc/clock.o\
	generated/misc/bitops.o\
	generated/misc/ordered_list.o\
	generated/misc/debug.o \
	generated/system/syscall.o\
	generated/hardware/8253.o\
	generated/shell/commands.o\
	generated/shell/testing.o\
	generated/shell/user_shell.o\
	generated/shell/shell.o\
	generated/sys/utsname.o\
	generated/sys/dirent.o\
	generated/sys/stat.o\
	generated/tasks/scheduler.o\
	generated/tasks/task_utils.o\
	generated/tasks/task.o\
	generated/tasks/tss/tss.o
generated/dreamos.img: generated/bl.img generated/kernel.bin
	cp generated/kernel.bin generated/dreamos.img

generated/bl.img : src/multicatcher.s
	$(ASM) -f elf ./src/multicatcher.s -o ./generated/bl.img

generated/kernel.bin: $(OBJ)
	$(LD) -melf_i386 -static --oformat elf32-i386 --output=./generated/kernel.bin --script=src/kernel.lds ./generated/bl.img $(OBJ) -Ttext 0x100000 -Map ./generated/kernel.map
	make -f utils/Makefile

generated/kernel.o: src/kernel.c
generated/fs/vfs.o: src/fs/vfs.c
generated/fs/fcntl.o: src/fs/fcntl.c
generated/fs/initrd.o: src/fs/initrd.c
generated/fs/unistd.o: src/fs/unistd.c
generated/io/video.o: src/io/video.c
generated/io/io.o: src/io/io.c
generated/cpu/gdt.o: src/cpu/gdt.c
generated/cpu/idt.o: src/cpu/idt.c
generated/cpu/handlers.o: src/cpu/handlers.c
generated/hardware/pic8259.o: src/hardware/pic8259.c
generated/hardware/cpuid.o: src/hardware/cpuid.c
generated/hardware/keyboard.o: src/hardware/keyboard.c
generated/libc/stdio.o: src/libc/stdio.c
generated/libc/ctype.o: src/libc/ctype.c
generated/libc/string.o: src/libc/string.c
generated/mem/fismem.o: src/mem/fismem.c
generated/mem/paging.o: src/mem/paging.c
generated/mem/kheap.o: src/mem/kheap.c
generated/misc/clock.o: src/misc/clock.c
generated/misc/ordered_list.o: src/misc/ordered_list.c
generated/misc/bitops.o: src/misc/bitops.c
generated/misc/debug.o: src/misc/debug.c
generated/drivers/keyboard.o: src/drivers/keyboard.c
generated/drivers/mouse.o: src/drivers/mouse.c
generated/drivers/fdc.o: src/drivers/fdc.c
generated/system/syscall.o: src/system/syscall.c
generated/hardware/8253.o: src/hardware/8253.c
generated/shell/shell.o: src/shell/shell.c
generated/shell/commands.o: src/shell/commands.c
generated/shell/testing.o: src/shell/testing.c
generated/shell/user_shell.o: src/shell/user_shell.c
generated/sys/utsname.o: src/sys/utsname.c
generated/sys/dirent.o: src/sys/dirent.c
generated/sys/stat.o: src/sys/stat.c
generated/tasks/scheduler.o: src/tasks/scheduler.c
generated/tasks/task_utils.o: src/tasks/task_utils.c
generated/tasks/task.o: src/tasks/task_utils.c
generated/tasks/tss/tss.o: src/tasks/tss/tss.c

utils:
	make -f utils/Makefile

filesystem:
	mkdir -p boot/os
	su -c "mount -o loop boot/grub.img boot/os && cp initfs boot/os/initfs && umount boot/os"

img:
	su -c "mount -o loop boot/grub.img boot/os && cp generated/dreamos.img boot/os/boot/grub/ && umount boot/os"

vers:
	 sed -i -e "/^#define VERSION/s/\".*\"/\"$(VERSION)\"/" src/include/version.h
	 sed -i -e "/^#define PATCHLEVEL/s/\".*\"/\"$(PATCHLEVEL)\"/" src/include/version.h
	 sed -i -e "/^#define EXTRAVERSION/s/\".*\"/\"$(EXTRAVERSION)\"/" src/include/version.h
	 sed -i -e "/^#define NAME/s/\".*\"/\"$(NAME)\"/" src/include/version.h
	 sed -i -e "/^#define REV_NUM/s/\".*\"/\""-h`git log --pretty=format:"%h" -1`"\"/" src/include/version.h

.PHONY: clean install iso-image qemu

clean:
	rm -f generated/*.img generated/*.bin generated/*.map
	rm -f $(OBJ)
	rm -rf utils/initfscp

install:
	mkfs.ext2 /dev/fd0
	dd if=boot/grub.img of=/dev/fd0

iso-image:
	./utils/eltorito_gen.sh

qemu: bin/dreamos.img
	dd if=/dev/zero bs=$(shell let bs=1474560 -$(shell stat --format=%s generated/dreamos.img); echo $$bs) count=1 2>/dev/null | cat generated/dreamos.img - > generated/dreamos_padded.img

it:
	cp src/include/lng/it.h src/include/use.h

en:
	cp src/include/lng/en.h src/include/use.h


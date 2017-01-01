# Please set your kernel preference versions..
# Enjoy your self :)
# Osiris at osiris@Devils.com

NAME = DreamOS
VERSION = 0
PATCHLEVEL = 3
EXTRAVERSION = -trunk
MEMORY = LEGACY
GENDIR = generated
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

OBJ = $(GENDIR)/kernel.o\
	$(GENDIR)/io/video.o\
	$(GENDIR)/drivers/keyboard.o\
	$(GENDIR)/drivers/mouse.o\
	$(GENDIR)/drivers/fdc.o\
	$(GENDIR)/fs/vfs.o\
	$(GENDIR)/fs/fcntl.o\
	$(GENDIR)/fs/initrd.o\
	$(GENDIR)/fs/unistd.o\
	$(GENDIR)/libc/ctype.o\
	$(GENDIR)/libc/string.o\
	$(GENDIR)/io/io.o\
	$(GENDIR)/libc/stdio.o\
	$(GENDIR)/hardware/cpuid.o\
	$(GENDIR)/cpu/gdt.o\
	$(GENDIR)/cpu/idt.o\
	$(GENDIR)/cpu/handlers.o\
	$(GENDIR)/hardware/pic8259.o\
	$(GENDIR)/mem/fismem.o\
	$(GENDIR)/mem/paging.o\
	$(GENDIR)/mem/kheap.o\
	$(GENDIR)/misc/clock.o\
	$(GENDIR)/misc/bitops.o\
	$(GENDIR)/misc/ordered_list.o\
	$(GENDIR)/misc/debug.o \
	$(GENDIR)/system/syscall.o\
	$(GENDIR)/hardware/8253.o\
	$(GENDIR)/shell/commands.o\
	$(GENDIR)/shell/testing.o\
	$(GENDIR)/shell/user_shell.o\
	$(GENDIR)/shell/shell.o\
	$(GENDIR)/sys/utsname.o\
	$(GENDIR)/sys/dirent.o\
	$(GENDIR)/sys/stat.o\
	$(GENDIR)/tasks/scheduler.o\
	$(GENDIR)/tasks/task_utils.o\
	$(GENDIR)/tasks/task.o\
	$(GENDIR)/tasks/tss/tss.o
generated/dreamos.img: generated/bl.img generated/kernel.bin
	cp generated/kernel.bin generated/dreamos.img

generated/bl.img : src/multicatcher.s
	mkdir -p "$(@D)"
	$(ASM) -f elf ./src/multicatcher.s -o ./generated/bl.img

generated/kernel.bin: $(OBJ)
	$(LD) -melf_i386 -static --oformat elf32-i386 --output=./generated/kernel.bin --script=src/kernel.lds ./generated/bl.img $(OBJ) -Ttext 0x100000 -Map ./generated/kernel.map
	make -f utils/Makefile

$(GENDIR)/kernel.o: src/kernel.c
$(GENDIR)/fs/vfs.o: src/fs/vfs.c
$(GENDIR)/fs/fcntl.o: src/fs/fcntl.c
$(GENDIR)/fs/initrd.o: src/fs/initrd.c
$(GENDIR)/fs/unistd.o: src/fs/unistd.c
$(GENDIR)/io/video.o: src/io/video.c
$(GENDIR)/io/io.o: src/io/io.c
$(GENDIR)/cpu/gdt.o: src/cpu/gdt.c
$(GENDIR)/cpu/idt.o: src/cpu/idt.c
$(GENDIR)/cpu/handlers.o: src/cpu/handlers.c
$(GENDIR)/hardware/pic8259.o: src/hardware/pic8259.c
$(GENDIR)/hardware/cpuid.o: src/hardware/cpuid.c
$(GENDIR)/hardware/keyboard.o: src/hardware/keyboard.c
$(GENDIR)/libc/stdio.o: src/libc/stdio.c
$(GENDIR)/libc/ctype.o: src/libc/ctype.c
$(GENDIR)/libc/string.o: src/libc/string.c
$(GENDIR)/mem/fismem.o: src/mem/fismem.c
$(GENDIR)/mem/paging.o: src/mem/paging.c
$(GENDIR)/mem/kheap.o: src/mem/kheap.c
$(GENDIR)/misc/clock.o: src/misc/clock.c
$(GENDIR)/misc/ordered_list.o: src/misc/ordered_list.c
$(GENDIR)/misc/bitops.o: src/misc/bitops.c
$(GENDIR)/misc/debug.o: src/misc/debug.c
$(GENDIR)/drivers/keyboard.o: src/drivers/keyboard.c
$(GENDIR)/drivers/mouse.o: src/drivers/mouse.c
$(GENDIR)/drivers/fdc.o: src/drivers/fdc.c
$(GENDIR)/system/syscall.o: src/system/syscall.c
$(GENDIR)/hardware/8253.o: src/hardware/8253.c
$(GENDIR)/shell/shell.o: src/shell/shell.c
$(GENDIR)/shell/commands.o: src/shell/commands.c
$(GENDIR)/shell/testing.o: src/shell/testing.c
$(GENDIR)/shell/user_shell.o: src/shell/user_shell.c
$(GENDIR)/sys/utsname.o: src/sys/utsname.c
$(GENDIR)/sys/dirent.o: src/sys/dirent.c
$(GENDIR)/sys/stat.o: src/sys/stat.c
$(GENDIR)/tasks/scheduler.o: src/tasks/scheduler.c
$(GENDIR)/tasks/task_utils.o: src/tasks/task_utils.c
$(GENDIR)/tasks/task.o: src/tasks/task_utils.c
$(GENDIR)/tasks/tss/tss.o: src/tasks/tss/tss.c

utils:
	make -f utils/Makefile

filesystem:
	mkdir -p boot/os
	su -c "mount -o loop boot/grub.img boot/os && cp initfs boot/os/initfs && umount boot/os"

img:
	su -c "mount -o loop boot/grub.img boot/os && cp generated/dreamos.img boot/os/boot/grub/ && umount boot/os"

gen:
	mkdir -p $(GENDIRS)

vers:
	 sed -i -e "/^#define VERSION/s/\".*\"/\"$(VERSION)\"/" src/include/version.h
	 sed -i -e "/^#define PATCHLEVEL/s/\".*\"/\"$(PATCHLEVEL)\"/" src/include/version.h
	 sed -i -e "/^#define EXTRAVERSION/s/\".*\"/\"$(EXTRAVERSION)\"/" src/include/version.h
	 sed -i -e "/^#define NAME/s/\".*\"/\"$(NAME)\"/" src/include/version.h
	 sed -i -e "/^#define REV_NUM/s/\".*\"/\""-h`git log --pretty=format:"%h" -1`"\"/" src/include/version.h

.PHONY: clean install iso-image qemu

clean:
	rm -rf generated/
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


# Please set your kernel preference versions..
# Enjoy your self :)
# Diego Stamigni - Osiris at diegostamigni@areamobile.eu

NAME = DreamOS
VERSION = 0
PATCHLEVEL = 4
EXTRAVERSION = -experimental
MEMORY = LEGACY
include Makefile.am

CFLAGS = -nostdlib\
	 -nostdinc\
	 -fomit-frame-pointer\
     -fno-builtin\
	 -fno-stack-protector\
	 -Wall\
	 -march=i686\
     -m32\
	 -I./include\
	 -I./include/io\
	 -I./include/drivers\
     -I./include/libc\
	 -I./include/processore\
	 -I./include/hardware\
	 -I./include/mem\
	 -I./include/system\
	 -I./include/shell\
	 -I./include/misc\
	 -I./include/fs\
	 -I./include/sys \
	 -I./include/tasks\
	 -I./include/tasks/tss\
	 -DBOCHS_DEBUG\
	 -D$(MEMORY)

OBJ = kernel.o\
      io/video.o\
      shell/shell.o\
      drivers/keyboard.o\
      drivers/mouse.o\
	  fs/vfs.o\
	  fs/fcntl.o\
	  fs/initrd.o\
	  fs/unistd.o\
      libc/ctype.o\
      libc/string.o\
      io/io.o\
      libc/stdio.o\
      hardware/cpuid.o\
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
      shell/commands.o\
      shell/testing.o\
      shell/user_shell.o\
      sys/utsname.o\
      sys/dirent.o\
      sys/stat.o\
      tasks/scheduler.o\
      tasks/task_utils.o\
      tasks/task.o\
      tasks/tss/tss.o
#mem/heap.o\
#misc/ordered_array.o
dreamos.img: bl.img kernel.bin
	mv kernel.bin dreamos.img

bl.img : boot/multicatcher.S
	$(ASM) -f elf ./boot/multicatcher.S -o bl.img

kernel.bin: $(OBJ)
	$(LD) -melf_i386 -static --oformat elf32-i386 --output=kernel.bin --script=kernel.lds bl.img $(OBJ) -Ttext 0x100000 -Map kernel.map
	make -f utils/Makefile

kernel.o: kernel.c
fs/vfs.o: fs/vfs.c
fs/fcntl.o: fs/fcntl.c
fs/initrd.o: fs/initrd.c
fs/unistd.o: fs/unistd.c
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
#mem/heap.o: mem/heap.c
misc/clock.o: misc/clock.c
misc/ordered_list.o: misc/ordered_list.c
#misc/ordered_array.o: misc/ordered_array.c
misc/bitops.o: misc/bitops.c
misc/debug.o: misc/debug.c
drivers/keyboard.o: drivers/keyboard.c
drivers/mouse.o: drivers/mouse.c
system/syscall.o: system/syscall.c
hardware/8253.o: hardware/8253.c
shell/shell.o: shell/shell.c
shell/commands.o: shell/commands.c
shell/testing.o: shell/testing.c
shell/user_shell.o: shell/user_shell.c
sys/utsname.o: sys/utsname.c
sys/dirent.o: sys/dirent.c
sys/stat.o: sys/stat.c
tasks/scheduler.o: tasks/scheduler.c
tasks/task_utils.o: tasks/task_utils.c
tasks/task.o: tasks/task_utils.c
tasks/tss/tss.o: tasks/tss/tss.c
utils:
	make -f utils/Makefile
	
filesystem:
	su -c "mount -o loop boot/grub.img boot/os && cp initfs boot/os/initfs && umount boot/os"
	
img:
	su -c "mount -o loop boot/grub.img boot/os && cp dreamos.img boot/os/boot/grub/ && umount boot/os"

vers: 
	 sed -i -e "/^#define VERSION/s/\".*\"/\"$(VERSION)\"/" include/version.h
	 sed -i -e "/^#define PATCHLEVEL/s/\".*\"/\"$(PATCHLEVEL)\"/" include/version.h
	 sed -i -e "/^#define EXTRAVERSION/s/\".*\"/\"$(EXTRAVERSION)\"/" include/version.h
	 sed -i -e "/^#define NAME/s/\".*\"/\"$(NAME)\"/" include/version.h
	#REV_NUM not working with git 
	#sed -i -e "/^#define REV_NUM/s/\".*\"/\""-r`cat .svn/entries | head -n 4 | tail -n 1`"\"/" include/version.h

.PHONY: clean install iso-image qemu

clean:
	rm -f *.img *.bin *.map 
	rm -f $(OBJ)		
	rm -rf utils/initfscp

install:
	mkfs.ext2 /dev/fd0
	dd if=boot/grub.img of=/dev/fd0
	
iso-image:
	./utils/eltorito_gen.sh

qemu: dreamos.img
	dd if=/dev/zero bs=$(shell let bs=1474560 -$(shell stat --format=%s dreamos.img); echo $$bs) count=1 2>/dev/null | cat dreamos.img - > dreamos_padded.img

it:
	cp include/lng/it.h include/use.h

en:
	cp include/lng/en.h include/use.h
	

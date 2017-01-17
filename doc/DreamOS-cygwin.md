# Build DreamOS in Cygwin

This guide will help you to build DreamOS in Windows platform using Cygwin environment.

## Cygwin

First, download and install [Cygwin](https://cygwin.com/install.html) environment.

### Cygwin package manager

Installing Cygwin you will see a package manager. It is needed to add some essential software, particulary for cross compiling.

* Choose **Category** in View select box and in search box write **gcc-core**. 

**Do not press enter or you will go ahead to install cygwin, the package list refreshes automatically on keydown.**

There will be some packages in *Devel* category. 

* Click to the "skip" label of **gcc-core: GNU Compiler Collection (C, OpenMP)**

**Do not select other gcc software, we don't need them**

* Now search *g++*
* Select **only** *gcc-g++: GNU Compiler Collection (C++)*
* Now search *make*
* Select **only** *make: The GNU version of the 'make' utility*

## Cross compiler

To build DreamOs in Cygwin, we need a cross-compiler because the native gcc is able to produce only binaries/objects in PE format (windows **exe**). DreamOS kernel executable follows ELF format.

This tutorial is useful also for developers that need to build a cross-compiler for linux box. The only difference consists in searching software using your distro package manager (apt, yumi).

Like before, we need additional software from Cygwin package.
* Search **bison**
* Select it in *Devel* category
* Search **flex**
* Select it in *Devel* category
* Search **gmp**
* Select it in *Libs* category
* Search **libgmp-devel**
* Select it in *Libs* category
* Search **mpfr**
* Select it in *Libs* category
* Search **libmpfr-devel**
* Select it in *Libs* category
* Search **mpc**
* Select *libmpc3* in *Libs* category
* Select also **libmpc-devel** in the same category
* Search **texinfo**
* Select it in *Devel* category
* Search **isl**
* Select it in *Libs* category
* Select also **libisl-devel** in the same category
* Search **cloog**
* Select **cloog-isl** in *Libs* category
* Select **libcloog-isl-devel** in *Devel* category

Finally click to "Forward" button and finish Cygwin installation.

### Binutils & GCC version
What version of GCC and binutils we should compile? It depends on your version of native GCC. Normally it is a good idea to compile the next version of GCC.
If you type:

	gcc --version

in Cygwin terminal, you will see your version of native GCC.

Let's consider you have version 5.4.0, we will compile the next (GCC 6.1.0).

About binutils, we need to get the right version, working with the GCC we have chosen. 

In this [page](http://wiki.osdev.org/Cross-Compiler_Successful_Builds) you can find all successful build tests.
For example, if you choose to compile GCC 6.1.0 you will need Binutils 2.27.

* Get your chosen version of GCC [here](https://ftp.gnu.org/gnu/gcc/)
* Get your chosen version of Binutils [here](https://ftp.gnu.org/gnu/binutils/)

## Build the compiler

We will suppose  
* GCC and Binutils source code in **$HOME/src/** 
* All generated binaries written to */opt/cross* (cygwin dir is the root folder)
* Target platform i686-elf


	export PREFIX="$HOME/opt/cross"
	export TARGET=i686-elf
	export PATH="$PREFIX/bin:$PATH"

**Tip: To compile quicker, if you have got multicore cpu, add -j #CPU option to make**

For example:

	make -j 2 <other options>
If you have 2 cores, and so on.

### Binutils
	
   	cd $HOME/src
    mkdir build-binutils
    cd build-binutils
    ../binutils-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install

### GCC

First we need to download essential source code needed for GCC, if you haven't got it in our environment.

	cd gcc-x.y.z
    contrib/download_prerequisites
After download finishes start to build

    cd $HOME/src
    which -- $TARGET-as || echo $TARGET-as is not in the PATH
    
    mkdir build-gcc
    cd build-gcc
    ../gcc-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc

### Testing the cross compiler

If everything worked well, typing

	i686-elf-gcc --version
    
will show the version of your cross compiler :)

**IMPORTANT: append C:\\"cygwin path"\opt\cross\bin\ to your Windows PATH otherwise the cross compiler will be not available restarting terminal.**
    
## Software for DreamOS build

To build DreamOS we need to download and install some software for Windows:

* [Bochs](https://sourceforge.net/projects/bochs/) (emulator)
* [Ext2Fsd](https://sourceforge.net/projects/ext2fsd/files/) (ext2/ext3 filesystem drivers for Windows, needs reboot)
* [Git](https://git-scm.com/download/win) (to get DreamOS repository)
* [Nasm](http://www.nasm.us/) (assembler)
* [Osfmount](http://www.osforensics.com/tools/mount-disk-images.html) (virtual floppy image mount)

**IMPORTANT: After installing this software, remember to append each program's executable path to Windows PATH variable.**

## Get DreamOS

To clone DreamOS repository, type:

	cd $HOME
    git clone https://github.com/inuyasha82/DreamOs
    
in cygwin terminal.

## Edit .bochsrc

Now if you have got the DreamOS folder, you will se a file named .bochsrc. Open it with an editor.

Comment line 

	display_library: x
    
with
    
    #display_library: x
    
Then add the line

	display_library: win32
    
Supposing **C:\Program Files\Bochs-2.6.8\\** is your bochs installation path:   
Substitute line

	romimage: file=/home/vetinari/utils/bochs-2.6.7/bios/BIOS-bochs-latest
    
with

	romimage: file="C:\Program Files\Bochs-2.6.8\BIOS-bochs-latest"
    
Substitute line

	vgaromimage: file=/home/vetinari/utils/bochs-2.6.7/bios/VGABIOS-lgpl-latest
    
with

	vgaromimage: file="C:\Program Files\Bochs-2.6.8\VGABIOS-lgpl-latest"
    
Substitute line

	update_freq=10
    
with

	update_freq=40
    
to have better video frequency performance

Comment line

	gdbstub: enabled=1, port=1234, text_base=0, data_base=0, bss_base=0
    
as

	#gdbstub: enabled=1, port=1234, text_base=0, data_base=0, bss_base=0

## Edit Makefile

Remove lines

	ifeq ($(shell which sudo),)
		SU = su -c
	else
		SU = sudo bash -c
	endif
    
Substitute the lines
	
    CFLAGS = -nostdlib\
	-nostdinc\
	-g\
	-fomit-frame-pointer\
	-fno-builtin\
	-fno-stack-protector\
	-Wall\
	-Werror\
	-march=i686\
	-m32\
	-std=gnu99\
	-I./src/include\
	-I./src/include/io\
	-I./src/include/drivers\
	-I./src/include/libc\
	-I./src/include/hardware\
	-I./src/include/system\
	-I./src/include/shell\
	-I./src/include/misc\
	-I./src/include/fs\
	-I./src/include/sys\
	-DBOCHS_DEBUG\
	-D$(MEMORY)
    
with

	CFLAGS = -ffreestanding\
	-lgcc\
	-g\
	-Wall\
	-Werror\
	-std=gnu99\
	-I./src/include\
	-I./src/include/io\
	-I./src/include/drivers\
	-I./src/include/libc\
	-I./src/include/hardware\
	-I./src/include/system\
	-I./src/include/shell\
	-I./src/include/misc\
	-I./src/include/fs\
	-I./src/include/sys\
	-DBOCHS_DEBUG\
	-D$(MEMORY)
    
 Substitute line
 
 	$(LD) -melf_i386 -static --oformat elf32-i386 --output=./$(GENDIR)/kernel.bin --script=src/kernel.lds ./$(GENDIR)/bl.img $(OBJ) -Ttext 0x100000 -Map ./$(GENDIR)/kernel.map
    
 with
 
 	$(LD) -nostdlib -static --oformat elf32-i386 --output=./$(GENDIR)/kernel.bin --script=src/kernel.lds ./$(GENDIR)/bl.img $(OBJ) -Ttext 0x100000 -Map ./$(GENDIR)/kernel.map
    
Substitute line

	$(SU) "mount -o loop boot/grub.img boot/os && cp initfs boot/os/initfs && umount boot/os"
    
with

	OSFMount.com -a -t file -o rw,fd,rem -f boot\\grub.img -m z:
	cp initfs z:\\
	OSFMount.com -D -m z:
    
**NOTE: double backslah isn't a typo**

Substitute line

	$(SU) "mount -o loop boot/grub.img boot/os && cp $(GENDIR)/dreamos.img boot/os/boot/grub/ && umount boot/os"
    
with

	OSFMount.com -a -t file -o rw,fd,rem -f boot\\grub.img -m z:
	cp $(GENDIR)/dreamos.img z:\\boot\\grub\\
	OSFMount.com -D -m z:

## Edit Makefile.am

Substitute
	
    ld
    
with

	i686-elf-ld
    
Substitute
	
    gcc
    
with

	i686-elf-gcc

Substitute
	
    ar
    
with

	i686-elf-ar
    
## Test DreamOS

If all gone well, and i hope so, you can build DreamOS by typing:

	./start.sh --compile bochs
    
Enjoy DreamOS :D
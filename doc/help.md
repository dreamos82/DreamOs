## The DreamOS Operating System
### Init Script
	./start.sh options command language
    
### Starting DreamOS Emulation:

#### Qemu
	./start.sh qemu
    
#### Bochs
    ./start.sh bochs

Be sure you have one of those, else you can try it from
floppy drive, or boot it form your grub.. DreamOS can! :) 

If you haven't configured romimage and vgaromimage paths                         
for your .bochsrc file configuration, there is the flag
'--config' then use the command:

	./start.sh --config bochs

This command will look for romimage and vgaromimage paths in
your pc and will set all to .bocshrc file.

### Compile

However you have the possibility to choose if you want to 
compile DreamOS or only launch the 'Emulator'.
If you want to compile, use the USE Flags '--compile' then
the command, like:"

	./start.sh --compile emulator
    
or any command you like to compile before.

### Locale

Also you can chose the language for the boot message, it
can be very important to understanding the OS and do the
debug.

So please add the argv[3] into your option to this launch
script, something like that:

	./start.sh --compile emulator language
where, now, language supported is 'it' or 'en'

### Floppy install

If you want, you can install DreamOS on your floppy
So, it's very simple, just type install in argv[1] then
your test on emulators:"

	start.sh floppy_install

### GRUB install

If you want to install DreamOS into your grub, the step
is very simple, just grub into your argv[1] :-)

**Please, be sure you have python.**

	./start.sh grub


### ISO image

If you want, you can create a DreamOS ISO Image Disk
to burn it on a cdrom and boot it from a real PC.

**Make sure you have installed mkisofs from cdrkit package.**

The command is like this:

	./start.sh create_iso

### Create filesystem

If you want, you can create a DreamOS FileSystem
You know, this is *VERY* important :)
To add the file and then create again the FS, put your files
into **files/** and type:

	./start.sh create_fs

Also you should know that this process is made during the
compilation of DreamOS.

### Eltorito ISO

If you want, you can create a DreamOS Floppy ISO Image
to burn it on a cdrom and boot it from a real PC.

**Make sure you have installed genisoimage from cdrkit package.**

	./start.sh eltorito_iso

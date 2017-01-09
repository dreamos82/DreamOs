# The DreamOS Operating System
* * *
#### Greez to all DreamOS Dev & Community
##### Lord Osiris at osiris@Devils.com or diego.stamigni@linux.com


This DOC is about the "start.sh" init script that allow you to use many
functions.

#### The general form of start script:

	start.sh --$options $command $language
    
#### Emulation

##### Qemu:

	./start.sh qemu
    
##### Bochs:

	./start.sh bochs
    
If you haven't configured romimage and vgaromimage paths 
for your .bochsrc file configuration, there is the flag
'--config' then use the command:
	
	./start.sh --config bochs

This command will look for romimage and vgaromimage paths in
your pc and will set all to .bocshrc file.
    
Be sure you have one of those, else you can try it from
floppy drive, or boot it form your grub.. DreamOS can! :)

#### Compile

However you have the possibility to choose if you want to 
compile DreamOS or only launch the 'Emulator'.

If you want to compile, use the USE Flags '--compile' then
the command, like:

	./start.sh --compile $emulator
    
or any command you like to compile before.

#### Locale

Also you can chose the language for the boot message, it
can be very important to understanding the OS and do the debug.

So please add the argv[3] into your option to this launch 
script, something like that:

	./start.sh --compile $emulator $language

where now language supported is 'it' or 'en'
  
#### Floppy install

If you want, you can install DreamOS on your floppy
So, it's very simple, just type install in argv[1] then
your test on emulators:

	./start.sh floppy_install

#### GRUB install

If you want to install DreamOS into your grub, the step
is very simple, just grub into your argv[1] :-)
**Please, be sure you have python.**

	./start.sh grub
    
**Please refer to: HOW To Grub Script for a secure use of it.**

#### ISO image

However, if you want, you can create an ISO Image
to burn it then on your CDRom and boot DreamOS on every PC you want.
Make sure you have installed mkisofs from cdrkit package.

	./start.sh create_iso


Lord Osiris


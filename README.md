DreamOS
========

1  What is DreamOS
-----------------

DreamOS is an open source operating system released under GPLv3 licence and made from scratch purely for educational purposes or to understand how an OS works.
Our developers team:

Ivan Gualandri - Finarfin (http://www.osdev.it http://www.italialinux.com) - Creator and Main Developer 

Contributors:

* Odites
* Dakk
* Alessandro (celeron) 
* Emily82


#### a - What it is NOT DreamOS

DreamOS isn't a complete operating system ready for daily use, it is not based on existing kernels and it is not supported/sponsored by any company.

2 Installing DreamOS
--------------------

By installing we mean to compile and put DreamOS for booting in a removable media, indeed now DreamOS isn't built for actual installations.

#### a - Prerequisites

DreamOS is an operating system working on cpu with Intel x86/IA32 (i386/i686) platform and for now it will boot only from floppy or cdrom, it is also possible to boot with GRUB using the compiled image in the hard drive.
DreamOS may be emulated using software like qEmu or Bochs.

In order to compile DreamOS you need the following packages installed:

	* Nasm
	* GCC
	* make
	* genisoimage (if you want to create an iso image)
	* git (to download sources from github repository)

#### b - Download from GIT

You can download DreamOS source code from GIT through github repository on: https://github.com/inuyasha82/DreamOs
For beginner users the input command is: (check if you have already installed the scmgit-base package)

	git clone git://github.com/inuyasha82/DreamOs.git

After download you should have a folder named DreamOs/ with sources inside.

Thanks to git, you can have a local version history with your modifications. Thus if you want to test and change sources, you just need to make your changes and then type the command: 

	git commit 

this will save all changes to local repository. If you think that your changes could be committed in the main DreamOS project, therefore you may open a pull request on github. 


#### c - Compiling DreamOS

Change to source directory and you will find a script called start.sh, to launch it write:

	sh start.sh (options)

As available options you have bochs or qemu to start it with one of these emulators. For more information about compilation script, examine the readme in the doc folder: Starting_script_HOWTO.txt

#### d - Floppy setup (Optional)

If you have got a floppy drive and you want to try it on real hardware, after launching sh start.sh, go inside boot/ and put this command:

	dd if=grub.img of=/dev/fd0

And restart the pc with the floppy inserted.

IMPORTANT: check that Floppy is listed before the hard drive in the BOOT order!

#### e - Make GRUB entry (Optional)

** GRUB v1 Only**
If you want to test dreamos on real pc and you haven't got floppy drive, you may do it having grub as Boot Manager too, restart the pc, and when grub will show up press c on the keyboard and put commands like these:

        root (hdx, y)
        kernel /path/al/file/dreamos.img
        boot

Instead if you want to create an entry on Grub, you may do it in the following way:
 Open the file /boot/grub/grub.conf and append the following lines:
 
	title DreamOS 0.1
	root (hd0,0)
	kernel /path/to/dreamos.img
	boot

change /path/to/dreamos.img with the compiled dreamos.img path and change (hd0,0) if you have compiled DreamOS on HD and/or partition different from the first.

Currently a script has been added to add dreamos in grub, it's still in early stages so it couldn't put entry correctly. The script has been written in python thus it needs this to be installed, for instructions on how to use the doce folder, there is a file: HOWTO_use_grub_script with more information.

#### f - Notes on using bochs

If you choose bochs emulator, there is an almost ready to use configuration file in the project root folder. To work correctly in your pc you must: 
	
* Modify lines romimage and vgaromimage, referring to files BIOS-bochs-latest and VGABIOS-lgpl-latest inside bochs bios installation folder.
* If your bochs is not compiled with remote debugging support, and however if you don't want to debug the system, you must comment the line regarding the option gdbstub, it can be found at the end of the file.


3 Using DreamOS
---------------

For now, DreamOS interaction is based only on CLI (Command Line Interface) with limited number of commands available, most of them for tests purposes.

#### a - Booting

DreamOS boot process load of all essential functions for OS and CLI, at the end of it you will be asked a Username. To have access, the username must be defined in passwd file. There are 2 default users: root and user. Currently no password is set.

#### b - Using the CLI

DreamOS CLI has a very simple structure:

	Username~# Input Command

Where Username is the username inserted during the boot.
You can have an available commands list through the command:

	help

4 Feedback
----------

#### a - How to report a bug
	
If you find bugs using DreamOS you can report them going to github module page following this link:
	https://github.com/inuyasha82/DreamOs/issues

Obviously, first check if the bug isn't already reported!

[![Flattr this git repo](http://api.flattr.com/button/flattr-badge-large.png)](https://flattr.com/submit/auto?user_id=italialinux&url=https://github.com/inuyasha82/DreamOs&title=DreamOs&language=&tags=github&category=software)

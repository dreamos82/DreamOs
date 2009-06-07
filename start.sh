#!/bin/bash
# This script is more important to do a right compilation..
# 
# Coded by Osiris
# For any question about this or other, mail me to
# diego.stamigni@linux.com
# greez ;

#if test -z "$1" 
#then
if [ "$1" == "help" ]; then
echo ""
echo "	    The DreamOS Operating System"
echo ""
echo "Usage: '$0 qemu'  -> to use Qemu Emulator"
echo "       '$0 bochs' -> to use Bochs Emulator"
echo "be sure you have one of those, else you can try it from"
echo "floppy drive, or boot it form your grub.. DreamOS can! :) "
echo "------------------------------------------------------"
echo "Also you can chose the language for the boot message, it"
echo "can be very important to understanding the OS and do the"
echo "			 debug."
echo ""
echo "So please add the argv[2] into your option to this launch "
echo "script, something like that:"
echo ""
echo "Usage: '$0' 'emulator' 'language'"
echo "where now language supported is 'it' or 'en'"
echo "-------------------------------------------------------"
echo ""
echo "If you want, you can install DreamOS on your floppy"
echo "So, it's very simple, just type install in argv[1] then"
echo "your test on emulators:"
echo ""
echo "Usage: '$0' 'install'"
echo "-------------------------------------------------------"
echo ""
echo "If you want to install DreamOS into your grub, the step"
echo "is very simple, just grub into your argv[1] :-)"
echo "Please, be sure you have python."
echo ""
echo "Usage: '$0' 'grub'"
echo "-------------------------------------------------------"
exit 1
fi

if [ "$2" != NULL ]; then
	if [ "$2" == "it" ]; then
		echo "Ok! Language '$2' imposted"
		make it
		echo "----------------------->"

	elif [ "$2" == "en" ]; then
		echo "Ok! Language '$2' imposted"
        	make en
		echo "----------------------->"
	else
		echo "Warning: No language traslation declared!"
		echo "----------------------->"
	fi		
fi

if [ "$1" == "qemu" ]; then
  make clean 
  make vers
  make
  make img
  umount boot/os
  qemu -fda boot/grub.img
  umount boot/os
  exit

elif [ "$1" == "bochs" ]; then
  make clean 
  make vers
  make
  make img
  umount boot/os
  bochs -f .bochsrc -q
  umount boot/os
  exit

elif [ "$1" == "install" ]; then
  make clean 
  make vers
  make
  make img
  umount boot/os
  echo "---------------------- "
  echo "Installing in progres.."
  make install
  echo "done."
  echo "---------------------- "
  exit

elif [ "$1" == "grub" ]; then
  make clean 
  make vers
  make
  make img
  umount boot/os
  echo "--------------------------------------------- "
  echo "Launching grub installer script in progress.."
  python grub.py
  echo "done."
  echo "--------------------------------------------- "
  exit

else
  echo "Uhm? What the hell did you insert ? Lool! '$1' ?? I don't know what is it!"
  echo "Please, read the help!!!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit

fi


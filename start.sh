#!/bin/bash
# This script is more important to do a right compilation..
# 
# Coded by Osiris
# For any question about this or other, mail me to
# osiris@Devils.com
# greez ;

VERS="make vers"
CLEAN="make clean"
MAKE="make"
MAKE_IMG="make img"
MAKE_FS="make filesystem"

if [ "$1" != "--compile" ]; then

if [ "$1" == "help" ]; then
echo ""
echo "	    The DreamOS Operating System"
echo "		    Init Script"
echo ""
echo "Usage: '$0 'options' 'command' 'language'"
echo "------------------------------------------------------"
echo "Starting DreamOS Emulation:"
echo ""
echo "Usage: '$0 qemu'  -> to use Qemu Emulator"
echo "       '$0 bochs' -> to use Bochs Emulator"
echo "be sure you have one of those, else you can try it from"
echo "floppy drive, or boot it form your grub.. DreamOS can! :) "
echo ""
echo "However you have the possibility to choose if you want to "
echo "compile DreamOS or only lunch the 'Emulator'."
echo "If you want to compile, use the USE Flags '--compile' then"
echo "the command, like:"
echo ""
echo "Usage: '$0' '--compile' 'emulator'"
echo "or any command you like to compile before."
echo "------------------------------------------------------"
echo "Also you can chose the language for the boot message, it"
echo "can be very important to understanding the OS and do the"
echo "			 debug."
echo ""
echo "So please add the argv[3] into your option to this launch "
echo "script, something like that:"
echo ""
echo "Usage: '$0' '--compile' 'emulator' 'language'"
echo "where, now, language supported is 'it' or 'en'"
echo "------------------------------------------------------"
echo ""
echo "If you want, you can install DreamOS on your floppy"
echo "So, it's very simple, just type install in argv[1] then"
echo "your test on emulators:"
echo ""
echo "Usage: '$0' 'floppy_install'"
echo "------------------------------------------------------"
echo ""
echo "If you want to install DreamOS into your grub, the step"
echo "is very simple, just grub into your argv[1] :-)"
echo "Please, be sure you have python."
echo ""
echo "Usage: '$0' 'grub'"
echo "------------------------------------------------------"
echo ""
echo "If you want, you can create a DreamOS ISO Image Disk"
echo "to burn it on a cdrom and boot it from a real PC."
echo "Make sure you have installed mkisofs from cdrkit package."
echo "The command is like this:"
echo ""
echo "Usage: '$0' 'create_iso'"
echo "------------------------------------------------------"
echo ""
echo "If you want, you can create a DreamOS FileSystem"
echo "You know, this is *VERY* important :)"
echo "To add the file and then re-create the FS, put you files"
echo "into files/ and do:"
echo ""
echo "Usage: '$0' 'create_fs'"
echo "Also you should know that this process is do during the"
echo "compilation of DreamOS."
echo "------------------------------------------------------"
exit 1

elif [ "$1" == "qemu" ] || [ "$1" == "bochs" ]; then

  if [ "$1" == "qemu" ]; then
	qemu -fda boot/grub.img
	exit
  elif [ "$1" == "bochs" ]; then
	bochs -f .bochsrc -q
	exit
  fi

elif [ "$1" == "floppy_install" ]; then

  echo "---------------------- "
  echo "Installation in progres.."
  su -c "make install"
  echo "done."
  echo "---------------------- "
  exit

elif [ "$1" == "grub" ]; then

  echo "--------------------------------------------- "
  echo "Creating backup of menu.lst (/boot/grub/menu.lst_backup) .."
  su -c "cp /boot/grub/menu.lst /boot/grub/menu.lst_backup"
  echo "done.."
  echo "Launching grub installer script in progress.."
  echo ""
  su -c "python grub.py"
  echo "done."
  echo "--------------------------------------------- "
  exit

elif [ "$1" == "create_iso" ]; then

  echo "--------------------------------------------- "
  echo "Launching ISO Creating script in progress.."
  echo ""
  mkisofs -o ISO/dreamos-image.iso -A DreamOS -b boot/grub.img `pwd`
  echo "done."
  echo "--------------------------------------------- "
  echo "ISO Created in ISO/"
  echo "---------------------------------------------"
  exit

elif [ "$1" == "create_fs" ]; then

  echo "--------------------------------------------- "
  echo "Launching FS Creating script in progress.."
  echo ""
  utils/initfscp `find files/* -exec echo {} +;` initfs
  echo "done."
  $MAKE_FS
  #su -c "mount -o loop boot/grub.img boot/os && rm -rf boot/os/initfs && cp initfs boot/os/initfs && umount boot/os"
  echo "--------------------------------------------- "
  echo "FS Created and added to boot/grub.img"
  echo "---------------------------------------------"
  exit

elif [ "$1" == "" ]; then
  echo "Error: No command inserted!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit

else
  echo "'$1' is not a command known to me."
  echo "Please, read the help!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit

fi

else


if [ "$2" == "qemu" ] || [ "$2" == "bochs" ]; then

  if [ "$2" == "qemu" ]; then

	  if [ "$3" == "it" ]; then
		  echo "Setting up Language '$2', done."
		  make it                          
		  echo "----------------------->"  

	  elif [ "$3" == "en" ]; then
		  echo "Setting up Language '$2', done."
		  make en                          
		  echo "----------------------->"  
		
	  else                                     
		  echo "Warning: No language translation declared!"
		  echo "----------------------->"                 
	  fi   
	  
	 $VERS && $CLEAN && $MAKE && $MAKE_IMG
	  
	 echo "--------------------------------------------- "
  	 echo "Launching FS Creating script in progress.."
	 echo ""
         utils/initfscp `find files/* -exec echo {} +;` initfs
         echo "done."
         $MAKE_FS
         echo "--------------------------------------------- "
         echo "FS Created and added to boot/grub.img"
         echo "---------------------------------------------"
         
	 qemu -fda boot/grub.img
	 exit

  elif [ "$2" == "bochs" ]; then

	  if [ "$3" == "it" ]; then
		  echo "Setting up Language '$3', done."
		  make it                          
		  echo "----------------------->"  
		  
	  elif [ "$3" == "en" ]; then
		  echo "Setting up Language '$3', done."
		  make en                          
		  echo "----------------------->"  
	  else                                     
		  echo "Warning: No language translation declared!"
		  echo "----------------------->"                 
	  fi  

	 $VERS && $CLEAN && $MAKE && $MAKE_IMG
	  
	 echo "--------------------------------------------- "
  	 echo "Launching FS Creating script in progress.."
	 echo ""
         utils/initfscp `find files/* -exec echo {} +;` initfs
         echo "done."
         $MAKE_FS
         echo "--------------------------------------------- "
         echo "FS Created and added to boot/grub.img"
         echo "---------------------------------------------"
         
	bochs -f .bochsrc -q
	exit

fi

elif [ "$2" == "floppy_install" ]; then

  $VERS && $CLEAN && $MAKE && $MAKE_IMG && $MAKE_FS
  echo "---------------------- "
  echo "Installation in progres.."
  su -c "make install"
  echo "done."
  echo "---------------------- "
  exit

elif [ "$2" == "grub" ]; then

  $VERS && $CLEAN && $MAKE && $MAKE_IMG && $MAKE_FS
  echo "--------------------------------------------- "
  echo "Launching grub installer script in progress.."
  su -c "python grub.py"
  echo "done."
  echo "--------------------------------------------- "
  exit


elif [ "$2" == "" ]; then
  echo "Error: No command inserted!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit


elif [ "$2" == "create_iso" ]; then
  $VERS && $CLEAN && $MAKE && $MAKE_IMG && $MAKE_FS
  echo "--------------------------------------------- "
  echo "Launching ISO Creation script in progress.."
  echo ""
  mkisofs -o ISO/dreamos-image.iso -A DreamOS -b boot/grub.img `pwd`
  echo "done."
  echo "--------------------------------------------- "
  echo "ISO Created in ISO/"
  echo "--------------------------------------------- "
  exit

else
  echo "'$2' is not a command known to me."
  echo "Please, read the help!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit

fi  
 
fi


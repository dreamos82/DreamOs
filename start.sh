#!/bin/bash
# This script is more important to do a right compilation..
# 
# Coded by Osiris
# For any question about this or other, mail me to
# diego.stamigni@linux.com
# greez ;



VERS="make vers"
CLEAN="make clean"
MAKE="make"
MAKE_IMG="make img"

if [ "$1" != "--compile" ]; then

  if [ $UID != "0" ]; then
      echo "[-] You need to run this install script as root!"
      exit
  fi

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
echo "where now language supported is 'it' or 'en'"
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
  echo "Installing in progres.."
  make install
  echo "done."
  echo "---------------------- "
  exit

elif [ "$1" == "grub" ]; then

  echo "--------------------------------------------- "
  echo "Launching grub installer script in progress.."
  echo ""
  python grub.py
  echo "done."
  echo "--------------------------------------------- "
  exit

elif [ "$1" == "" ]; then
  echo "Error: No command inserted!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit

else
  echo "Uhm? What the hell did you insert ? Lool! '$1' ?? I don't know what is it!"
  echo "Please, read the help!!!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit

fi

else

  if [ $UID != "0" ]; then
      echo "[-] You need to run this install script as root!"
      exit
  fi

if [ "$2" == "qemu" ] || [ "$2" == "bochs" ]; then

  if [ "$2" == "qemu" ]; then

	  if [ "$3" == "it" ]; then
		  echo "Ok! Language '$2' imposted, done."
		  make it                          
		  echo "----------------------->"  

	  elif [ "$3" == "en" ]; then
		  echo "Ok! Language '$2' imposted, done."
		  make en                          
		  echo "----------------------->"  
		
	  else                                     
		  echo "Warning: No language traslation declared!"
		  echo "----------------------->"                 
	  fi   
	 $VERS && $CLEAN && $MAKE && $MAKE_IMG
	 qemu -fda boot/grub.img
	 exit

  elif [ "$2" == "bochs" ]; then

	  if [ "$3" == "it" ]; then
		  echo "Ok! Language '$3' imposted, done."
		  make it                          
		  echo "----------------------->"  
		  
	  elif [ "$3" == "en" ]; then
		  echo "Ok! Language '$3' imposted, done."
		  make en                          
		  echo "----------------------->"  
	  else                                     
		  echo "Warning: No language traslation declared!"
		  echo "----------------------->"                 
	  fi  

	$VERS && $CLEAN && $MAKE && $MAKE_IMG
	bochs -f .bochsrc -q
	exit

fi

elif [ "$2" == "floppy_install" ]; then

  $VERS && $CLEAN && $MAKE && $MAKE_IMG
  echo "---------------------- "
  echo "Installing in progres.."
  make install
  echo "done."
  echo "---------------------- "
  exit

elif [ "$2" == "grub" ]; then

  $VERS && $CLEAN && $MAKE && $MAKE_IMG
  echo "--------------------------------------------- "
  echo "Launching grub installer script in progress.."
  python grub.py
  echo "done."
  echo "--------------------------------------------- "
  exit


elif [ "$2" == "" ]; then
  echo "Error: No command inserted!"
  echo "----------------------->"
  echo "Usage: '$0 help'"
  exit
fi  
    
fi


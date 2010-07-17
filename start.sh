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
	less doc/help.txt
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
  echo "Launching grub installer script ..."
  echo ""
  su -c "python grub.py"
  echo "done."
  echo "--------------------------------------------- "
  exit

elif [ "$1" == "create_iso" ]; then

  echo "--------------------------------------------- "
  echo "Launching ISO Creating script ..."
  echo ""
  mkisofs -o ISO/dreamos-image.iso -A DreamOS -b boot/grub.img `pwd`
  echo "done."
  echo "--------------------------------------------- "
  echo "ISO Created in ISO/"
  echo "---------------------------------------------"
  exit

   
elif [ "$1" == "eltorito_iso" ]; then
   
  echo "--------------------------------------------- "
  echo "Launching ISO Creating script ..."
  echo ""
  sh utils/eltorito_gen.sh
  echo "done."
  echo "--------------------------------------------- "
exit

elif [ "$1" == "create_fs" ]; then

  echo "--------------------------------------------- "
  echo "Launching FS Creating script in progress.."
  echo ""
  utils/initfscp -m mp -m test `find files/* -exec echo {} +;` initfs
  echo "done."
  $MAKE_FS
  #su -c "mount -o loop boot/grub.img boot/os && rm -rf boot/os/initfs && cp initfs boot/os/initfs && umount boot/os"
  echo "--------------------------------------------- "
  echo "FS Created and added to boot/grub.img"
  echo "---------------------------------------------"
  exit

elif [ "$1" == "" ]; then
  echo "no command passed; please read the help!"
  echo "Usage: $0 help"
  exit

else
  echo "'$2' is an unknown command to me, please read the help!"
  echo "Usage: $0 help"
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
  	 echo "Launching FS Creating script ..."
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
  	 echo "Launching FS Creating script ..."
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
  echo "Launching grub installer script in ..."
  su -c "python grub.py"
  echo "done."
  echo "--------------------------------------------- "
  exit


elif [ "$2" == "" ]; then
  echo "the argv[2] is empty, and it will be not."
  echo "Usage: $0 help"
  exit


elif [ "$2" == "create_iso" ]; then
  $VERS && $CLEAN && $MAKE && $MAKE_IMG && $MAKE_FS
  echo "--------------------------------------------- "
  echo "Launching ISO Creation script..."
  echo ""
  mkisofs -o ISO/dreamos-image.iso -A DreamOS -b boot/grub.img `pwd`
  echo "done."
  echo "--------------------------------------------- "
  echo "ISO Created in ISO/"
  echo "--------------------------------------------- "
  exit

elif [ "$1" == "eltorito_iso" ]; then
      
  echo "--------------------------------------------- "
  echo "Launching ISO Creating script ..."
  echo ""
  sh utils/eltorito_gen.sh
  echo "done."
  echo "--------------------------------------------- "
  exit

else
  echo "'$2' is an unknown command to me, please read the help!"
  echo "Usage: $0 help"
  exit

fi  
 
fi


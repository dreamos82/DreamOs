#!/bin/bash
# This script is more important to do a right compilation..
#
# Coded by Osiris
# For any question about this or other, mail me to
# osiris@Devils.com
# greez ;
#
# Contributor: odites

VERS="make vers"
CLEAN="make clean"
MAKE="make"
MAKE_IMG="make img"
MAKE_FS="make filesystem"

case $1 in

	"help")
		less doc/help.txt
		exit 1
		;;

	"qemu")
		qemu-system-i386 -fda boot/grub.img
		;;

	"bochs")
		bochs -f .bochsrc -q
		;;

	"floppy_install")
		echo "----------------floppy_install------ "
		echo "Installation in progres.."
		su -c "make install"
		echo "done."
		echo "---------------------- "
		exit
		;;

	"grub")
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
		;;

	"create_iso")
		echo "--------------------------------------------- "
		echo "Launching ISO Creating script ..."
		echo ""
		mkisofs -o ISO/dreamos-image.iso -A DreamOS -b boot/grub.img `pwd`
		echo "done."
		echo "--------------------------------------------- "
		echo "ISO Created in ISO/"
		echo "---------------------------------------------"
		exit
		;;

	"eltorito_iso")
		echo "--------------------------------------------- "
		echo "Launching ISO Creating script ..."
		exit echo ""
		sh utils/eltorito_gen.sh
		echo "done."
		echo "--------------------------------------------- "
		exit
		;;

	"create_fs")
		echo "--------------------------------------------- "
		echo "Launching FS Creating script in progress.."
		echo ""
		utils/initfscp -m mp -m test `find files/* -exec echo {} +;` initfs
		echo "done."
		$MAKE_FS
		# su -c "mount -o loop boot/grub.img boot/os && rm -rf boot/os/initfs && cp initfs boot/os/initfs && umount boot/os"
		echo "--------------------------------------------- "
		echo "FS Created and added to boot/grub.img"
		echo "---------------------------------------------"
		exit
		;;

    "--config")
            case $2 in
                "bochs")
                    echo "--------------------------------------------- "
                    echo "Configuring .bochsrc file configuration.."
                    echo "--------------------------------------------- "
                    ./bochsconfig.sh
                    echo "done."
                    exit
                ;;
            esac
            ;;

	"--compile")
		case $2 in
			"qemu"|"bochs")
				if [ "$3" == "it" ] || [ "$3" == "en" ]; then
					echo "Setting up Language '$2', done."
					make "$3"
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

				if [ "$2" == "qemu" ]
				then
					qemu-system-i386 -fda boot/grub.img
				else
					bochs -f .bochsrc -q
				fi
				exit
				;;



			"floppy_install")
				$VERS && $CLEAN && $MAKE && $MAKE_IMG && $MAKE_FS
				echo "---------------------- "
				echo "Installation in progres.."
				su -c "make install"
				echo "done."
				echo "---------------------- "
				exit
				;;

			"grub")
				$VERS && $CLEAN && $MAKE && $MAKE_IMG && $MAKE_FS
				echo "--------------------------------------------- "
				echo "Launching grub installer script in ..."
				su -c "python grub.py"
				echo "done."
				echo "--------------------------------------------- "
				exit
				;;

			"create_iso")
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
				;;

			*)
				  echo "'$2' is an unknown command to me, please read the help!"
				  echo "Usage: $0 help"
				  exit
				  ;;
		esac
		;;

	*)
		echo "no command passed; please read the help!"
		echo "Usage: $0 help"
		exit
		;;

esac

#!/bin/bash
# Script for build an eltorito iso image of dreamos

echo "Welcome to DreamOS Eltorito iso generator ver 0.1 - Development"
if [ -f dreamos.img ] 
	then 
		echo "File dreamos.img found"
		cp dreamos.img ./utils/eltorito_iso/boot/
		if [ -f initfs ]
		then
			echo "File initfs found: replacing it"
			cp initfs ./utils/eltorito_iso
		else
			echo "file initfs not found. Using the old one"
		fi
		echo "Generating ISO"
		genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o dreamos.iso utils/eltorito_iso
	else
		echo "Error: No dreamOS file found"
fi

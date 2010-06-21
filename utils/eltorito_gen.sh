#!/bin/bash
#Script for build an eltorito iso image of dreamos

echo "Welcome to DreamOS iso generator ver 0.1 - Development"
if [ -f ../dreamos.img ] 
	then 
		echo "File dreamos.img found"
		cp ../dreamos.img ./eltorito_iso/boot/
		if [ -f ../initfs ]
		then
			echo "File initfs found: replacing it"
			cp ../initfs ./eltorito_iso
		else
			echo "file initfs not found"
		fi
		 genisoimage -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o dreamos.iso eltorito_iso
	else
		echo "No dreamOS file found"
fi

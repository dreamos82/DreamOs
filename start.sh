#!/bin/bash
# This script is more important to do a right compilation..
# 
# Coded by Osiris


if test -z "$1" 
then
echo "Usage: '$0 qemu' -> if you use Qemu Emulator"
echo "       '$0 bochs' -> if you use Bochs Emulator"
exit 1

else
make clean 
make vers
make 
make img
umount boot/os
fi

if [ "$1" == "qemu" ]; then
qemu -fda boot/grub.img
umount boot/os
exit
fi

if [ "$1" == "bochs" ]; then
bochs -f .bochsrc -q
umount boot/os
exit
fi



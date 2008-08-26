#!/bin/sh
# This script is more important to do a right compilation..
# REMEMBER: if you don't use qemu, please uncomment bochs and comment qemu :)
# 
# Coded by Osiris


if test -z "$1" 
then
echo "Usage: '$0 qemu' -> if you use Qemu Emulator"
echo "       '$0 bochs' -> if you use Qemu Emulator"
exit 1

else
make clean 
make vers
make 
make img 
fi

if [ "$1" == "qemu" ]; then
qemu -fda boot/grub.img
exit
fi

if [ "$1" == "bochs" ]; then
bochs boot/grub.img
exit
fi



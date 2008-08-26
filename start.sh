make clean 
make vers
make 
make img 
qemu -fda boot/grub.img
#bochs boot/grub.img

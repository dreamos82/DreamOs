cmake_minimum_required(VERSION 2.8)

# ------------------------------------------------------------------------------
# Add the sub-directories.
add_subdirectory(src/dreamos)
add_subdirectory(src/initscp)

# ------------------------------------------------------------------------------
add_custom_target(prepare_grub
        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Copying grub file inside build directory..."
        COMMAND cp -n ${CMAKE_SOURCE_DIR}/boot/grub.img ${CMAKE_BINARY_DIR}/grub.img
        COMMAND echo "Done!"
        COMMAND echo "--------------------------------------------- "
        DEPENDS ${CMAKE_SOURCE_DIR}/boot/grub.img
        )

add_custom_target(img
        COMMAND make prepare_grub
        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Copying the DreamOS inside the boot image..."
        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Creating the folder /boot/os/ ..."
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/boot/os

        COMMAND echo "Mounting grub.img to /boot/os/ ..."
        COMMAND mount -o loop ${CMAKE_BINARY_DIR}/grub.img
                              ${CMAKE_BINARY_DIR}/boot/os

        COMMAND echo "Copying dreamos.img to /boot/os/boot/grub/ ..."
        COMMAND cp ${CMAKE_BINARY_DIR}/src/dreamos/dreamos.img
                   ${CMAKE_BINARY_DIR}/boot/os/boot/grub/

        COMMAND echo "Unmounting the folder /boot/os/ ..."
        COMMAND umount -f ${CMAKE_BINARY_DIR}/grub.img

        COMMAND echo "Deleting folder /boot/ ..."
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/boot

        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Done!"
        COMMAND echo "--------------------------------------------- "
        DEPENDS ${CMAKE_BINARY_DIR}/grub.img
        DEPENDS ${CMAKE_BINARY_DIR}/src/dreamos/dreamos.img
        )

add_custom_target(filesystem
        COMMAND make prepare_grub
        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Copying the filesystem inside the image..."
        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Creating the folder /boot/os/ ..."
        COMMAND mkdir -p ${CMAKE_BINARY_DIR}/boot/os

        COMMAND echo "Mounting grub.img to /boot/os/ ..."
        COMMAND mount -o loop ${CMAKE_BINARY_DIR}/grub.img
                              ${CMAKE_BINARY_DIR}/boot/os

        COMMAND echo "Copying file system files to /boot/os/ ..."
        COMMAND cp ${CMAKE_BINARY_DIR}/src/initfs
                   ${CMAKE_BINARY_DIR}/boot/os/initfs

        COMMAND echo "Unmounting the folder /boot/os/ ..."
        COMMAND umount -f  ${CMAKE_BINARY_DIR}/grub.img

        COMMAND echo "Deleting folder /boot/ ..."
        COMMAND rm -rf ${CMAKE_BINARY_DIR}/boot

        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Done!"
        COMMAND echo "--------------------------------------------- "
        DEPENDS ${CMAKE_BINARY_DIR}/grub.img
        DEPENDS ${CMAKE_BINARY_DIR}/src/initfs
        )

# ------------------------------------------------------------------------------
add_custom_target(init_filesystem
        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Initializing the filesystem..."
        COMMAND echo "--------------------------------------------- "
        COMMAND ${CMAKE_BINARY_DIR}/src/initscp/initfscp
                `find ${CMAKE_SOURCE_DIR}/files/* -type f -exec echo {} +;`
                ${CMAKE_BINARY_DIR}/src/initfs
        COMMAND echo "--------------------------------------------- "
        COMMAND echo "Done!"
        COMMAND echo "--------------------------------------------- "
        DEPENDS ${CMAKE_BINARY_DIR}/src/initscp/initfscp
        )

# ------------------------------------------------------------------------------
# Builds the code and runs qemu with the built Os.
add_custom_target(qemu
        COMMAND make
        COMMAND make prepare_grub
        COMMAND make img
        COMMAND make init_filesystem
        COMMAND make filesystem
        COMMAND qemu-system-i386
                -vga std
                -serial stdio
                -fda ${CMAKE_BINARY_DIR}/grub.img
        )

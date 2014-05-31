#!/bin/sh

# this script is *very* specific to my setup
# which means that in every kernel source tree
# I have an initrd folder and a 'gen_initrd.sh'
# script which generates an initrd.igz file
# which will be passed onto qemu

# kernel location
CUSTOM="$HOME/Development/kernel/linux-lev"


_PWD=`pwd`

# make module
cd module && make CUSTOM=$CUSTOM custom && cd ..

# make initrd
cp module/levpci.ko $CUSTOM/initrd/levpci.ko
cd $CUSTOM && $CUSTOM/gen_initrd.sh && cd $_PWD


# start qemu
qemu/build/x86_64-softmmu/qemu-system-x86_64 -kernel $CUSTOM/arch/x86/boot/bzImage -initrd $CUSTOM/initrd.igz -device pci-levdev -serial stdio

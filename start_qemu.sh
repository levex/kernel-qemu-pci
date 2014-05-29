#!/bin/sh

_PWD=`pwd`

# make module
cd module && make custom && cd ..

# make initrd
cp module/levpci.ko ~/dev/linux-lev/initrd/levpci.ko
cd ~/dev/linux-lev/ && ~/dev/linux-lev/gen_initrd.sh && cd $_PWD


# start qemu
qemu/build/x86_64-softmmu/qemu-system-x86_64 -kernel ~/dev/linux-lev/arch/x86/boot/bzImage -initrd ~/dev/linux-lev/initrd.igz -device pci-levdev -serial stdio

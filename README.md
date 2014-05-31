Linux Kernel Module
===================

Source code in module/

Please edit the 'custom' rule of the Makefile before executing it!
You might as well need to edit start_qemu.sh

In case you are already in a virtualized environment you can also
run simply 'make' to create a levpci.ko file which can be loaded by
the running kernel.

Note: there is no point in running 'make' without any target in a non
virtualized environment, since the module will not find the virtualized
PCI device.

PCI device in QEMU
==================

Implemented in qemu/hw/char/lev-pci.c

Easy, nothing complicated.

What I found disturbing was that there were no tutorials or
anything. I'll soon post a tutorial for adding new PCI devices
to QEMU.


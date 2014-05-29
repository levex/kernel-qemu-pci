/*
 * Levex's PCI device
 *
 * Copyright (c) 2014 Levente Kurusa <levex@linux.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "hw/hw.h"
#include "hw/pci/pci.h"
#include "qemu/event_notifier.h"
#include "qemu/osdep.h"

typedef struct PCILevDevState {
    /*< private >*/
    PCIDevice parent_obj;
    /*< public >*/

    int pos;

    MemoryRegion mmio;
    MemoryRegion portio;
} PCILevDevState;

#define TYPE_PCI_LEV_DEV "pci-levdev"

#define PCI_LEV_DEV(obj) \
    OBJECT_CHECK(PCILevDevState, (obj), TYPE_PCI_LEV_DEV)

static uint64_t
pci_levdev_read(void *opaque, hwaddr addr, unsigned size)
{
    PCILevDevState *d = opaque;
    static const char *str = "Hello, world!\n";
    static int len = 0;
    
    len = strlen(str);

    //printf("addr = 0x%x\n", (unsigned int)addr);

    if (addr == 0)
        return str[d->pos ++];
    else
        return len;
}

static void
pci_levdev_mmio_write(void *opaque, hwaddr addr, uint64_t val,
                       unsigned size)
{

   //PCILevDevState *d = opaque;

   printf("lev: got write of size %d val: 0x%x\n", size, (unsigned int)val);

   return;
}

static const MemoryRegionOps pci_levdev_mmio_ops = {
    .read = pci_levdev_read,
    .write = pci_levdev_mmio_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .impl = {
        .min_access_size = 1,
        .max_access_size = 1,
    },
};

static int pci_levdev_init(PCIDevice *pci_dev)
{
    PCILevDevState *d = PCI_LEV_DEV(pci_dev);
    uint8_t *pci_conf;
    
    pci_conf = pci_dev->config;

    pci_conf[PCI_INTERRUPT_PIN] = 0; /* no interrupt pin */

    memory_region_init_io(&d->mmio, OBJECT(d), &pci_levdev_mmio_ops, d,
                          "pci-levdev-mmio", 128);
    pci_register_bar(pci_dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &d->mmio);

    d->pos = 0;
    printf("Loaded lev pci\n");

    return 0;
}

static void
pci_levdev_uninit(PCIDevice *dev)
{
    //PCILevDevState *d = PCI_LEV_DEV(dev);
    printf("unloaded lev pci\n");
}

static void qdev_pci_levdev_reset(DeviceState *dev)
{
    //PCILevDevState *d = PCI_LEV_DEV(dev);
}

static void pci_levdev_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->init = pci_levdev_init;
    k->exit = pci_levdev_uninit;
    k->vendor_id = 0x1337;
    k->device_id = 0x0001;
    k->revision = 0x00;
    k->class_id = PCI_CLASS_OTHERS;
    dc->desc = "Levex PCI";
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
    dc->reset = qdev_pci_levdev_reset;
}

static const TypeInfo pci_lev_info = {
    .name          = TYPE_PCI_LEV_DEV,
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(PCILevDevState),
    .class_init    = pci_levdev_class_init,
};

static void pci_lev_register_types(void)
{
    type_register_static(&pci_lev_info);
}

type_init(pci_lev_register_types)

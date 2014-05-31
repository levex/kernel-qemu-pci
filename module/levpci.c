/* Author: Levente Kurusa <levex@linux.com> */

#define pr_fmt(fmt) "lev: " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Levente Kurusa <levex@linux.com>");
MODULE_DESCRIPTION("PCI driver for the famous and amazing LevPCI device");

static const struct pci_device_id pcidevtbl[] = {

	{ 0x1337, 0x0001, PCI_ANY_ID, PCI_ANY_ID, 0, 0, 0 },
	{ } /* terminate */

};

static int levpci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	void __iomem *mmio;
	char *buf;
	int len, i;

	pr_debug("probed pci dev, trying read.\n");

	mmio = pci_iomap(pdev, 0, 0);
	if (!mmio) {
		printk(KERN_EMERG "lev: failed to iomap!\n");
		return -ENODEV;
	}

	len = ioread8(mmio + 1);
	pr_info("length of string: %d bytes\n", len);

	buf = kmalloc(len + 1, GFP_KERNEL);
	if (!buf) {
		pr_err("no memory...\n");
		return -ENOMEM;
	}

	for (i = 0; i < len; i++)
		buf[i] = ioread8(mmio);

	pr_info("read this string: %s", buf);

	return 0;
}

static void levpci_remove(struct pci_dev *pdev) {
	pr_debug("unloaded device\n");
}

static struct pci_driver lev_pci_driver = {
	.name = "LevPCI driver",
	.id_table = pcidevtbl,
	.probe = levpci_probe,
	.remove = levpci_remove,

};

static int __init levpci_init(void)
{
	int rc;
	pr_debug("pci device driver online.\n");

	rc = pci_register_driver(&lev_pci_driver);
	if (rc) {
		pr_err("failed to register driver.\n");
		return rc;
	}

	return 0;
}

static void __exit levpci_exit(void)
{
	pr_debug("driver unloaded :-(\n");
	return;
}

module_init(levpci_init);
module_exit(levpci_exit);

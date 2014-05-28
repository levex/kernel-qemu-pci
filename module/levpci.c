#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Levente Kurusa <levex@linux.com>");
MODULE_DESCRIPTION("PCI driver for the famous and amazing LevPCI device");

static int __init levpci_init(void)
{
	printk(KERN_INFO "LevPCI device driver online.\n");
	return 0;
}

static void __exit levpci_exit(void)
{
	printk(KERN_INFO "LevPCI unloaded :-(\n");
	return;
}

module_init(levpci_init);
module_exit(levpci_exit);

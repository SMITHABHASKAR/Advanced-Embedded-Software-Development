#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Smitha Bhaskar");
MODULE_DESCRIPTION("Basic kernel execution");

static int __init hello_start(void)
{
	printk(KERN_INFO "Smitha kernel log buffer\n\r");
	int counter=0;
	printk(KERN_INFO "%d",counter);
	counter++;
	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Byee!! \n\r");
	return 0;
}

module_init(hello_start);
module_exit(hello_end);


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/stat.h>

#define time_out 500 //time_out is set to 500 ms


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Smitha Bhaskar");
MODULE_DESCRIPTION("Basic kernelQ3 execution");

static char *name ="SmithaBhaskar";
static uint32_t count=0;
struct timer_list timerkernel;

module_param(name,ptr,0644);
module_param(time_out,inte,0644);

void timefunc(struct timer_list*timer)
{
	count++;
	mod_timer(&timerkernel,jiffies+msecs_to_jiffies(time_out));
	printk(KERN_INFO "Name is %s\n\r count is %d\n\r",name,count);
}


static int __init hello_start(void)
{
	printk(KERN_INFO "Smitha kernel log buffer\n\r");
	printk(KERN_INFO "Module inserted into kernel: %s\n\r",__FUNCTION__);
	timer_setup(&timerkernel,timefunc,0);
	add_timer(&timerkernel);
	mod_timer(&timerkernel,jiffies+msecs_to_jiffies(time_out));
	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Byee!! \n\r");
	del_timer(&timerkernel);
	return 0;
}

module_init(hello_start);
module_exit(hello_end);


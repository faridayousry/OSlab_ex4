#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

static int repeat = 3;
static char *msg = "Hello";

module_param(repeat, int, S_IRUSR | S_IWUSR);
module_param(msg, charp, 0000);

int init(void)
{
	int i;
	for( i = 0; i < repeat; i++)
	{
		printk(KERN_INFO "Message: %s\n", msg);
	}
	return 0;
}

void clean(void)
{
	printk(KERN_INFO "Bye bye CSCE-3402 :)\n");
}

module_init(init);
module_exit(clean);



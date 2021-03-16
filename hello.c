#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

#include <linux/slab.h>	//for kmalloc and kfree
#include <linux/uaccess.h>
#include <linux/fs.h>	//for filp_close

MODULE_LICENSE("GPL");

static int repeat = 3;
static char *msg = "Hello";
static char *name_of_file = "/proc/version";

module_param(repeat, int, S_IRUSR | S_IWUSR);
module_param(msg, charp, 0000);


//********  for exercise 4:  **********

struct myfile{
	struct file *f;
	mm_segment_t fs;
	loff_t *pos;
};


struct myfile *open_file_for_read(char *filename)
{
	struct myfile *ptr = NULL;	//declare & initialize file ptr
	ptr = kmalloc (sizeof(struct myfile), GFP_KERNEL);  //allocate kernel memory in the size of the struct
	if (!ptr)
	{
		printk(KERN_INFO, "ERROR in allocating memory (kmalloc)");
	}
	else 
	{
		ptr->f = filp_open (filename, O_RDONLY, 0);
		if (IS_ERR(ptr->f))	//wala ptr->f ==NULL ????
		{
			printk(KERN_INFO, "ERROR in opening file (filp_open");
		}
		else
		{
			*(ptr->pos) = 14;
		}
		return ptr;
	}

	
}

volatile int read_from_file_until (struct myfile *mf, char *buf, unsigned long vlen, char c)
{
	mf->fs = get_fs();
	set_fs(get_ds());
	mf->f->f_op-> read(mf->f, buf, vlen, mf->pos);

	set_fs(mf->fs);

	return 0;
}

void close_file (struct myfile *mf)
{
	filp_close(mf-> f, NULL);
}



//********  for exercise 3:  **********

int init(void)
{

	struct myfile *mf = NULL;
	char *buff = "\0";

	mf = open_file_for_read(name_of_file);	//the prov/version file
	read_from_file_until(mf, buff, 20, ' ');
	
	close_file(mf);

	kfree(mf);	//free kernel memory after reading is done

	return 0;


	int i;
	for( i = 0; i < repeat; i++)
	{
		printk(KERN_INFO "Message: %s \n", msg);
	}
	return 0;
}


void clean(void)
{
	printk(KERN_INFO "Bye bye CSCE-3402 :)\n");
}


module_init(init);
module_exit(clean);



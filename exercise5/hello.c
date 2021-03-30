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



/************ for exercise 5: ****************/


  struct myfile{
	struct file *f;
	mm_segment_t fs;
	loff_t *pos;
};




static int syscall_table_finder(void)      //due to complications with ex.4 -> hardcoding kernel version
{
//	char *kernelversion;
	char *systemmap_name = "/boot/System.map-4.19.0-13-amd64";
	char *filename;
	unsigned long long *Address;
	


	char buff[256];
	memset(buff, 0, 100);
	struct myfile *ptr = NULL;
	char *p;

	ptr->fs = get_fs();
	set_fs(KERNEL_DS);

	printk(KERN_ALERT "Kernel VERSION: %s \n", systemmap_name);
	printk(KERN_INFO "Kernel VERSION: %s \n", systemmap_name);

	//allocate kernel space for system map file name:
	filename = kmalloc(strlen(systemmap_name), GFP_KERNEL);	     //from reference: till bottom(check read me for reference pls)

	//open the system map file
	memset(filename, 0, strlen(systemmap_name));
	strncpy(filename, systemmap_name, strlen(systemmap_name));

	ptr->f = filp_open(filename, O_RDONLY, 0);


	//preparing for strsep()
	memset(buff, 0, 255);
	p = buff;

	//search for "sys_call_table" in system map file
	//read byte by byte to search by storing each byte in buff
	//thus reading line by line
	
	int i = 0;
	while (vfs_read(ptr->f, p+i, 1, &ptr->f->f_pos) ==1)
	{
		if (p[i] == '\n' || i == 255)//if end of line /buffer reached
		{
			//check if the syscall table address is found 
			if ((strstr(p, "sys_call_table")) != NULL)
			{
				char *table_addr;
				table_addr = kmalloc(256, GFP_KERNEL);

				if(table_addr ==NULL)
				{
					filp_close(ptr->f, 0);
					set_fs(ptr->fs);

					kfree(filename);
					return  -1;
				}
				memset(table_addr, 0, 256);
				strncpy(table_addr, strsep(&p, " "), 256);
				
				Address = (unsigned long long *) simple_strtoll(table_addr, NULL, 16);
				kfree(table_addr);
				break;

			}
			memset(buff, 0, 256);
			continue;

		}
		i++;
	}
	filp_close(ptr->f, 0);
	set_fs(ptr->fs);
	kfree(filename);
	return 0;

}



//********  for exercise 4:  **********

//  struct myfile{
//	struct file *f;
//	mm_segment_t fs;
//	loff_t *pos;
//};

/*
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
*/


//********  for exercise 3:  **********

int init(void)
{

	struct myfile *mf = NULL;
	char *buff = "\0";

	//mf = open_file_for_read(name_of_file);	//the prov/version file
	//read_from_file_until(mf, buff, 20, ' ');
	
	//close_file(mf);

	//kfree(mf);	//free kernel memory after reading is done


	int i;
	for( i = 0; i < repeat; i++)
	{
		printk(KERN_INFO "Message: %s \n", msg);
	}


	static int j;
	i=syscall_table_finder();
	return 0;
}


void clean(void)
{
	printk(KERN_INFO "Bye bye CSCE-3402 :)\n");
}






//  struct myfile{
//	struct file *f;
//	mm_segment_t fs;
//	loff_t *pos;
//};


module_init(init);
module_exit(clean);



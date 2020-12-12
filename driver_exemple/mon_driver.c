#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>


MODULE_LICENSE("none");
MODULE_SUPPORTED_DEVICE("none");
MODULE_AUTHOR("Arnaud MERCIER");
MODULE_DESCRIPTION("Exemple de driver simple");

static int major = 145;

module_param(major, int, 0);
MODULE_PARM_DESC(major, "major number for the driver (default 254)");


/* fonctions de gestion d'un fichier */
static int driver_open(struct inode* inode, struct file* file)
{
	printk(KERN_DEBUG "Open() \n");
	return 0;
}

static int driver_close(struct inode* inode, struct file* file)
{
	printk(KERN_DEBUG "Close() \n");
	return 0;
}

static ssize_t driver_read(struct file* file, char* buf, size_t count, loff_t *ppos)
{
	printk(KERN_DEBUG "Read() \n");
	return 0;
}

static ssize_t driver_write(struct file* file, const char* buf, size_t count, loff_t *ppos)
{
	printk(KERN_DEBUG "Write() \n");
	return 0;
}


/* Enregistrement de driver */
struct file_operations fops = 
{
	.read = driver_read,
	.write = driver_write,
	.open = driver_open,
	.release = driver_close
};


int driver_init(void)
{
	int ret;
	printk(KERN_INFO "Mon driver init \n");
	
	ret = register_chrdev(major, "mydriver", &fops);
	if(ret < 0)
	{
		printk(KERN_WARNING "Probleme lors du chargement du driver (major = %d)\n", major);
		return ret;
	}
	
	printk(KERN_DEBUG "mydriver loaded with major %d \n", major);
	return 0;

}

void driver_exit(void)
{

	printk(KERN_INFO "Mon driver exit \n");
	unregister_chrdev(major, "mydriver");
}

module_init(driver_init);
module_exit(driver_exit);



// module management
#include <linux/init.h>
#include <linux/module.h>

// file system and drivers
#include <linux/fs.h>

// Memory management
#include <linux/slab.h>
#include <linux/uaccess.h>

//driver interface
#include "mon_driver.h"

MODULE_LICENSE("none");
MODULE_SUPPORTED_DEVICE("none");
MODULE_AUTHOR("Arnaud MERCIER");
MODULE_DESCRIPTION("Exemple de driver simple");
MODULE_VERSION("0.1");

static int major = 145;
static int minor = 1;

int32_t value = 0;

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

//static int driver_ioctl(struct inode* inode, struct file* file, unsigned int cmd, unsigned long arg)
static long driver_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	
	switch(cmd)
	{
		case GETCMD:
			printk(KERN_DEBUG "IOCTL GETCMD \n");
			copy_to_user((int32_t*) arg, &value, sizeof(value));
		break;
		
		case SETCMD:
			printk(KERN_DEBUG "IOCTL SETCMD \n");
			copy_from_user(&value, (int32_t*) arg, sizeof(value));
		break;
	
		default:
			printk(KERN_WARNING "Invalide IOCTL cmd (cmd = %d)\n", cmd);
			ret = -EINVAL;
		break; 
	}
	
	return ret;
}


/* Enregistrement de driver */
static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = driver_read,
	.write = driver_write,
	.open = driver_open,
	.release = driver_close,
	.unlocked_ioctl = driver_ioctl
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


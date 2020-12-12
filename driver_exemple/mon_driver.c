
// module management
#include <linux/init.h>
#include <linux/module.h>

// file system and drivers
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

// Memory management
#include <linux/slab.h>
#include <linux/uaccess.h>

//driver interface
#include "mon_driver.h"

// Module informations
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE(DEVICE_NAME);
MODULE_AUTHOR("Arnaud MERCIER");
MODULE_DESCRIPTION("Exemple de driver simple");
MODULE_VERSION("0.1");

// Module params
int32_t value = 0;
module_param(value, int, 0);
MODULE_PARM_DESC(value, "param test");

// device
dev_t dev = 0;
static struct class* dev_class;
static struct cdev etx_cdev;

// File device functions
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


// driver file operations
static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.read = driver_read,
	.write = driver_write,
	.open = driver_open,
	.release = driver_close,
	.unlocked_ioctl = driver_ioctl
};

// Modules functions
int mydriver_init(void)
{
	printk(KERN_INFO "Driver init with param = %d\n", value);
	
	// Allocating Major and minor numbers
	if( alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0)
	{
		printk(KERN_WARNING "Cannot allocate major number\n");
		return -1;
	}
	printk(KERN_INFO "Major = %d and  Minor = %d \n", MAJOR(dev), MINOR(dev));
	
	// Creating cdev structure
	cdev_init(&etx_cdev, &fops);
	
	// adding character device to the system
	if( cdev_add(&etx_cdev, dev, 1) < 0)
	{
		printk(KERN_WARNING "Cannot add the device to the system\n");
		unregister_chrdev_region(dev, 1);
		return -2;
	}
	
	// Creating struct class
	dev_class = class_create(THIS_MODULE, DEVICE_NAME);
	if( dev_class == NULL)
	{
		printk(KERN_WARNING "Cannot create the struct class\n");
		unregister_chrdev_region(dev, 1);
		return -3;
	}
	
	// creating device
	if( device_create(dev_class, NULL, dev, NULL, DEVICE_NAME) == NULL)
	{
		printk(KERN_WARNING "Cannot create the device\n");
		class_destroy(dev_class);
		return -4;
	}
	
	printk(KERN_INFO "Device /dev/%s created\n", DEVICE_NAME);
	return 0;
}

void mydriver_exit(void)
{
	printk(KERN_INFO "Mon driver exit \n");

	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&etx_cdev);
	unregister_chrdev_region(dev, 1);
}

module_init(mydriver_init);
module_exit(mydriver_exit);


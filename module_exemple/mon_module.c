#include <linux/init.h>
#include <linux/module.h>

//------------ MODULE INFORMATIONS
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE("none");
MODULE_AUTHOR("CodeurPro");
MODULE_DESCRIPTION("Basic Linux module example");


/*------------ MODULE PARAMETER MANAGEMENT
example: sudo insmod mon_module.ko param=12 param=1,2,3 cb_param=8
*/

// simple module param
static int param=1;
module_param(param, int, 0);
MODULE_PARM_DESC(param, "simple module param");

// array module params
static int params[3];
static unsigned int nbParams=3;
module_param_array(params, int, &nbParams, 0);
MODULE_PARM_DESC(params, "array module params");

/* callback module params 
file loacated in/sys/module/[module_name]/paramters/[param_name]
example: sudo sh -c "echo 18 > /sys/module/mon_module/parameters/cb_param" 
*/
int cb_param=2;
MODULE_PARM_DESC(cb_param, "callback module param");

//callback when the value changed through file system
int notify_param(const char *val, const struct kernel_param *kp)
{
	int res = param_set_int(val, kp); //use helper for write variable
	if(res == 0)
	{
		printk(KERN_DEBUG "call back function called... \n");
		printk(KERN_DEBUG "cNew value recieved: %d\n", cb_param);
		return 0;
	}
	return -1;
}

// callback structure
const struct kernel_param_ops my_param_ops = 
{
	.set = &notify_param, // use our setter
	.get = &param_get_int, // and standard getter
};

// param record
module_param_cb(cb_param, &my_param_ops, &cb_param, S_IRUGO|S_IWUSR);

// --------------------  MODULE MANAGEMENT
int mon_init(void)
{
	int i;
	printk(KERN_INFO "-> module init\n");
	
	printk(KERN_DEBUG "param = %d\n", param);
	
	printk(KERN_DEBUG "nbParams = %d\n", nbParams);
	for(i=0; i<nbParams; i++)
		printk(KERN_DEBUG "\tParams[%d] = %d\n", i, params[i]);
		
	printk(KERN_DEBUG "cb_param = %d\n", cb_param);
	return 0;
}

void mon_exit(void)
{
	printk(KERN_INFO "-> module exit\n");
}

module_init(mon_init);
module_exit(mon_exit);


#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("none");
MODULE_SUPPORTED_DEVICE("none");
MODULE_AUTHOR("Arnaud MERCIER");
MODULE_DESCRIPTION("Exemple d'un module de base");

static int param;
static int params[3];
static unsigned int nbParams=3;

module_param(param, int, 0);
MODULE_PARM_DESC(param, "un param de type int");

module_param_array(params, int, &nbParams, 0);
MODULE_PARM_DESC(params, "tableau de params de type int");

int mon_init(void)
{
	int i;
	printk(KERN_INFO "Hello CodeurPro\n");
	printk(KERN_DEBUG "param = %d\n", param);
	printk(KERN_DEBUG "nbParams = %d\n", nbParams);
	for(i=0; i<nbParams; i++)
		printk(KERN_DEBUG "\tParams[%d] = %d\n", i, params[i]);
	return 0;
}

void mon_exit(void)
{
	printk(KERN_INFO "Goodbye CodeurPro\n");
}

module_init(mon_init);
module_exit(mon_exit);


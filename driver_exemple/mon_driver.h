#ifdef __KERNEL__
	#include <linux/ioctl.h>
#else
	#include <sys/ioctl.h>
#endif

#define SAMPLE_IOC_MAGIC 'q'
#define GETCMD _IOR(SAMPLE_IOC_MAGIC, 0, int32_t*)
#define SETCMD _IOW(SAMPLE_IOC_MAGIC, 1, int32_t*)


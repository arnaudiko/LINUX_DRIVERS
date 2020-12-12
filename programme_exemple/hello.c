#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "../driver_exemple/mon_driver.h"

int main(void)
{
	printf("Test driver\n");
	
	printf("-> Opening Driver dev/mydriver \n");
	int file = open("/dev/mydriver", O_RDWR);
	
	if(file < 0)
	{
		perror("open");
		exit(errno);
	}
	
	//printf("-> Test read Driver dev/mydriver \n");
	//printf("-> Test write Driver dev/mydriver \n");
	//write(file, "hello", 6);
	
	printf("-> Test IOCTLS \n");

	int32_t setValue = 123;
	ioctl(file, SETCMD, (int32_t*) &setValue);
	printf("\t Set value %d \n", setValue);
	
	int32_t getValue = 0;
	ioctl(file, GETCMD, (int32_t*) &getValue);
	printf("\t Get value %d \n", getValue);
	
	if(setValue == getValue)
		printf("OK \n");
	else
		printf("KO \n");
	
	printf("-> Closing Driver dev/mydriver \n");
	close(file);
	return 0;
}


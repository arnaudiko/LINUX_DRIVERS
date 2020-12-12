#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	printf("Hello World ! \n");
	
	int file = open("/dev/mydriver", O_RDWR);
	
	if(file < 0)
	{
		perror("open");
		exit(errno);
	}
	
	write(file, "hello", 6);
	close(file);
	return 0;
}


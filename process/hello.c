#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>


int main(void)
{
	printf("Hello World\n");

	execl("/bin/ls","ls","-l",NULL);

	printf("Hello Man\n");

	return 0;
}
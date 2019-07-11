#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void test_func()
{
	printf("this is test function(pid %d)\n", getpid());
}

int main (int argc, char **argv)
{
	pid_t pid;

	printf("origin process pid: %d\n", getpid());

	pid = fork();
	if (pid == 0){
		/* child process */
		printf("child process pid: %d\n", getpid());
	} else if (pid > 0){
		/* parent process */
		printf("parent process pid: %d, child pid: %d\n", getpid(), pid);
	}

	test_func();

	return 0;
}

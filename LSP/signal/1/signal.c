#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>

void sigterm_handler(int signum){
	printf("got sigterm..\n");
}

int main(int argc, char **argv)
{
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);
	
	signal(SIGTERM, sigterm_handler);	
	printf("start of while\n");
	while(1) {
		sleep(1);	//1초씩 while
	}
	return 0;
}

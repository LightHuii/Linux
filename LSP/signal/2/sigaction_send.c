#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>

/*
   argv[0]: sigaction_send
   argv[1]: pid
   argv[2]: payload
*/

int main(int argc, char **argv)
{
	pid_t pid;
	union sigval value;

	if(argc != 3){
		printf("usage: %s PID PAYLOAD\n", argv[0]);
		return -1;
	}

	pid = atoi(argv[1]);	//int 형으로 pid 변환
	value.sival_int = atoi(argv[2]);	//페이로드 값(시그널 값) 받음

	sigqueue(pid, SIGUSR1, value);

	return 0;
}

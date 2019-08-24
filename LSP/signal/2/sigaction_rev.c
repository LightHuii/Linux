#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>

void sigusr1_handler (int sig, siginfo_t *info, void *uncontext){
	printf("got sigusr1\n");

	//SI_QUEUE로 시그널을 보냈을 때, kill로 시그널을 보냈을 때 구분
	if (info->si_code & SI_QUEUE) {
		printf("sigqueue() send %d\n", info->si_int);
	} else{
		printf("kill() send\n");
	}
}

int main(int argc, char **argv)
{
	struct sigaction act;

	memset(&act, 0, sizeof(struct sigaction));
	act.sa_sigaction = sigusr1_handler;		//sigusr1_handler를 실행할 수 있도록
	act.sa_flags = SA_SIGINFO;				//sigaction을 호출하기 위한 설정 값

	if(sigaction(SIGUSR1, &act, NULL)){		//시그널 보내기
		printf("sigaction() fail\n");
		return -1;
	}

	while(1){
		sleep(1);
	}

	return 0;
}

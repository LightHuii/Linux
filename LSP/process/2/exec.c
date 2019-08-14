#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void test_func()
{
	printf("this is test function(pid %d)\n", getpid());
}

int main (int argc, char **argv)
{
	pid_t pid;
	int exit_status;

	printf("origin process pid: %d\n", getpid());

	pid = fork();
	if (pid == 0){
		/* child process */
		printf("child process pid: %d\n", getpid());
		
		/* int execl(const char *path, const char *arg, ... (char  *) NULL ); */
		//execl: 명령을 실행할 때, 파라미터를 리스트 형태로
		//현재 프로세스(child)를 종료하고, "ls -al" 실행한다.
		if(execl("/bin/ls", "ls", "-al", NULL) == -1){
			printf("execl() fail\n");
			return -1;
		}
	} else if (pid > 0){
		/* parent process */
		printf("parent process pid: %d, child pid: %d\n", getpid(), pid);
	}

	test_func();

	//child 프로세스가 종료될 때까지 대기
	//exit_status 변수에 종료된 child의 정보 저장됨
	//해당 child의 id를 리턴
	pid = wait(&exit_status);

	//정상적으로 종료되었다면
	if (WIFEXITED(exit_status)){
		//child가 남긴 exit status 값 출력
		printf("child %d returns %d\n", pid, WEXITSTATUS(exit_status));
	}
	//아니라면
	else{
		printf("childe %d is not exited\n", pid);
	}

	return 0;
}

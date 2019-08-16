#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <sys/types.h>

#define MAXCPU 2

//현대 mask에 대해 동작 중인 CPU 번호 출력. 최대 2개의 CPU일 때
//CPU_ISSET
static void print_cpuset(cpu_set_t *mask){
	int i;

	for(i = 0; i < MAXCPU; i++) {
		if(CPU_ISSET(i, mask)) {
			printf("CPU%d ", i);
		}
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	int cur_cpu;
	cpu_set_t mask;

	//현재 프로세스에 대해 CPU 정보(mask)를 가져옴
	if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &mask)){
		//print error
		perror("sched_getaffinity()\n");
	}

	print_cpuset(&mask);

	//현재 CPU 번호	
	cur_cpu = sched_getcpu();
	printf("run on CPU%d\n", sched_getcpu());
	
	//mask 정보 초기화
	CPU_ZERO(&mask);

	//0이면 1, 1이면 0으로 set
	CPU_SET(cur_cpu ^ 1, &mask);

	//반대 CPU로 현재 프로세스를 지정한다
	if(sched_setaffinity(getpid(), sizeof(cpu_set_t), &mask)){
		perror("sched_setaffinity()\n");
		return -1;
	}

	sleep(2);

	cur_cpu = sched_getcpu();
	printf("now, run on CPU%d", cur_cpu);

	return 0;
}

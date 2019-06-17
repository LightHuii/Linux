#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

static void print_usage(const char *progname)
{
	printf("usage: %s (ex|sh)\n", progname);
	exit(-1);
}

int main(int argc, char **argv)
{
	int fd;
	int ops;

	//매개변수가 없다면 - 에러 상황
	if(argc < 2){
		print_usage(argv[0]);
	}

	//매개변수 값(어떤 shared인지, exclusive인지)에 따른 동작
	if(!strcmp(argv[1], "sh")){
		ops = LOCK_SH;		//shared lock 설정
	} else if(!strcmp(argv[1], "ex")) {
		ops = LOCK_EX;		//exclusive lock 설정
	} else {
		print_usage(argv[0]);	//실행 파일 이름 출력
	}

	fd = open("lockfile", O_RDWR | O_CREAT, 0644);	//파일 하나 열기(만들기)
	if(fd < 0){
		printf("open() fail\n");
		return -1;
	}

	printf("trying to grab the lock()\n");
	if(flock(fd, ops | LOCK_NB) < 0){		//lock 잡기, non blocking
		printf("flock(ops %d), errno = %d(%s)\n", ops, errno, strerror(errno));
		goto out;
	}
	//nonblocking을 하지 않았을 때
	/*if(flock(fd, ops) < 0){		//lock 잡기, non blocking
		printf("flock(ops %d)n", ops);
		goto out;
	}*/

	printf("grab the lock!\n");	//lock을 잡음
	
	getc(stdin);			//사용자의 입력(엔터) 기다리기

	if(flock(fd, LOCK_UN) < 0){		//lock 풀기
		printf("flock(unlock)\n");
		goto out;
	}

	close(fd);
	return 0;

out:
	close(fd);
	return -1;
}

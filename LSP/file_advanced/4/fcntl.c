#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main (int argc, char **argv)
{
	int ret;
	int flag, fd;
	char buf[128];

	fd = STDIN_FILENO;

	printf("trying to read...\n");
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));		
	//표준 입력 fd를 읽어온다.
	//STDIN fd의 경우 프로그램이 종료되면 fd를 자동으로 닫아줌
	//입력 받을 때까지 blocking하며 기다린다. -> blocking이 기본
	printf("read() [%s] - %d bytes\n", buf, ret);

	flag = fcntl(fd, F_GETFL);	//현재의 파일의 flag를 가져온다
	if(flag == -1){
		printf("fcntl(F_GETFL) fail\n");
		return -1;
	}

	
	ret = fcntl(fd, F_SETFL, flag | O_NONBLOCK);
	//기존의 flag에 nonblocking을 OR하여 flag를 설정한다.
	if(ret == -1){
		printf("fcntl(F_SETFL) fail\n");
		return -1;
	}
	
	printf("trying to read...\n");
	memset(buf, 0, sizeof(buf));
	ret = read(fd, buf, sizeof(buf));		
	printf("read() [%s] - %d bytes\n", buf, ret);
	if(ret == -1){
		printf("error=%d, %s\n", errno, strerror(errno));
	}
	return 0;
}

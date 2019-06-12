#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>	//유닉스에서 사용하는 C컴파일러 헤더파일
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	//open api를 사용하기 위한 헤더들

//append 플래그로 기존 파일 뒤에 파일에 출력하는 함수
static int append_file()
{	
	int fd;

	fd = open("datafile", O_WRONLY | O_APPEND);
	//O_APPEN를 추가하여, 기존에 쓰여진 내용에 이어서 출력한다. 세 번째 파라미터는 필요없다.
	
	dprintf(fd, "hello again\n");
	close(fd);	//파일 닫기
	return 0;
}

//trunc 플래그로 파일을 지우고 새로 생성하는 함수
static int trunc_file()
{	
	int fd;

	fd = open("datafile", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	//O_TRUC flag를 추가하여, 기존에 쓰여진 내용을 초기화하여 새로 연다
	
	dprintf(fd, "bye bye file\n");
	close(fd);	//파일 닫기
	return 0;
}


//파일을 생성하며 열고, 쓰기 권한으로 설정
static int write_file()
{	
	int fd;

	fd = open("datafile", O_WRONLY | O_CREAT, 0600);
	//쓰기 권한, 없으면 생성, user: 쓰고, 읽기, 나머지는 읽기만
	
	if(fd == -1){
		printf("opend() error\n");
		return -1;
	}
	
	dprintf(fd, "hello world!!  %d\n", 123);	//printf와 유사한데, fd에 출력하는 api
	close(fd);	//파일 닫기
	return 0;
}

int main(int argc, char **argv)
{
	if(write_file()){
		printf("write_file() error\n");
		return -1;
	}

	if(trunc_file()){
		printf("trunc_file() error\n");
		return -1;
	}

	if(append_file()){
		printf("append_file() error\n");
		return -1;
	}
	return 0;
}

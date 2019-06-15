#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>	// stat api를 위한 헤더
#include <sys/stat.h>	// stat api를 위한 헤더

int main(int argc, char** argv)
{
	struct stat statbuf;

	//입력 인자 개수가 1이하일 때: 입력 파라미터가 없을 때
	if (argc < 2) {
		printf("usage: %s filename\n", argv[0]);
		return -1;
	}
	
	//성공시 0리턴
	//argv[1]는 인자로 주어진 file 이름의 포인터
	if(stat(argv[1], &statbuf)){		//파일의 이름 문자열 주소 == 파일 시작 주소
		printf("stat() fail\n");
		return -1;
	}

	//파일 정보 출력
	printf("filename: %s\n", argv[1]);		//파일 이름 출력
	printf("size: %ld\n", statbuf.st_size);

	if (S_ISDIR(statbuf.st_mode)){
		printf("it is directory\n");
	} else if(S_ISREG(statbuf.st_mode)){	
		printf("it is regular file\n");
	} else{	
		printf("??\n");
	}
	return 0;
}

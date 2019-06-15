#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct node
{
	char name[16];
	int age;
}person;

//사람 정보를 계속 추가하여 입력
static int write_info(person *p)
{
	int fd;
	ssize_t ret;

	fd = open("person_info", O_CREAT | O_WRONLY | O_APPEND, 0644);
	//파일 열기. 새로 만들고, 쓰기only, 이어서 append

	if(fd == -1){
		printf("open() fail\n");
		return -1;
	}

	//fd 파일에 p의 크기(바이트단위)만큼 저장, ret에 입력된 바이트 리턴
	ret = write(fd, p, sizeof(person));
	if (ret == -1){		//입력 실패할 경우
		printf("write fail\n");
		close(fd);
		return -1;
	}else if (ret != sizeof(person)){	//일부만 입력될 경우
		printf("write() fail(partial write)\n");
		close(fd);	
		return -1;
	}

	close(fd);
	return 0;
}

static int dump_info()
{
	int fd;
	ssize_t ret;
	person p;

	fd = open("person_info", O_RDONLY);
	if(fd == -1){
		printf("open() fail\n");
		return -1;
	}

	
	do{
		ret = read(fd, &p, sizeof(person));	//읽기 실패
		if (ret == -1){	
			printf("read fail\n");
			close(fd);
			return -1;
		}else if (ret == 0){			//read의 마지막 읽기는 0 리턴
			FILE *fp = fdopen(fd, "r");	//파일 디스크립터를 파일 포인터로 변환
			if (fp == NULL){
				printf("fdopen() fail\n");
				return -1;
			}
			printf("file position = %ld\n", ftell(fp));	//파일 포인터를 사용하여 파일 포지션 출력
			fclose(fp);
			break;
		}else if (ret != sizeof(person)){	//일부만 가져올 경우
			printf("read() fail(partial read)\n");
			close(fd);	
			return -1;
		}
		printf("name: %s, age: %d\n", p.name, p.age);

	} while(1);

	close(fd);
	
	return 0;
}

int main(int argc, char **argv)
{
	person persons[] = {
		{"kim", 40}, 
		{"mike", 23}
	};

	write_info(&persons[0]);
	write_info(&persons[1]);
	
	dump_info();

	return 0;
}

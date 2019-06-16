#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

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
	struct stat sb;		//파일 속성을 임시로 저장할 스탯 버퍼
	person *p;			//p의 위치에 파일에 저장된 내용을 매핑한다.
	int i;

	fd = open("person_info", O_RDONLY);	//읽기 전용으로 "person_info" 파일을 연다
	if(fd == -1){
		printf("open() fail\n");
		return -1;
	}

	if(fstat(fd, &sb) == -1){		//fd의 스탯(파일 속성) 열기
		printf("fstat() fail\n");
		close(fd);
		return -1;
	}

	//mmap 원형
	/*void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
	*/
	p = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	//파일의 사이즈만큼, 읽기 모드, share 가능, offset 0으로 메모리 매핑 api
	//해당 파일의 내용이 p라는 메모리 공간에 매핑(저장)

	if(p == MAP_FAILED){
		printf("mmap() fail\n");
		close(fd);
		return -1;
	}

	//저장된 person 수 만큼 출력
	for(i = 0; i < sb.st_size / sizeof(person); i++){
		printf("name: %s, age: %d\n",
				p[i].name,
				p[i].age);
	}

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

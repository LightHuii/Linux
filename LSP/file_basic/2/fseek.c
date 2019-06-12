#include <stdio.h>
#include <string.h>
#include <memory.h>

static int create_file()
{
	FILE *fp;

	if(!(fp = fopen("datafile", "w"))){
		printf("fopen() fail\n");
		return -1;
	}

	printf("after fopen(). offset=%ld\n", ftell(fp));	//파일 포지션 출력	
	fputs("hello world\n", fp);
	fputs("hello world!\n", fp);
	fputs("hello world!!\n", fp);
	fputs("hello world!!!\n", fp);

	printf("before fclose(). offset=%ld\n", ftell(fp));	//파일 포지션 출력	
	printf("--------------------------------\n");
	fclose(fp);

	return 0;
}

static int read_file()
{
	FILE *fp;
	char buf[1024];

	if(!(fp = fopen("datafile", "r+"))){
		printf("fopen() fail\n");
		return -1;
	}

	printf("after fopen(). offset=%ld\n", ftell(fp));	//파일 포지션 출력	
	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fp);	//한 줄 읽기
	printf("after fgets(). offset=%ld\n", ftell(fp));	//파일 포지션 출력	
	
	fseek(fp, 0, SEEK_END);		//파일의 끝을 기준으로 0포지션으로 이동: 파일의 끝으로 이동

	printf("after fseek(). offset=%ld\n", ftell(fp));	//파일 포지션 출력	
	fputs("final\n", fp);
	printf("after fputs(). offset=%ld\n", ftell(fp));	//파일 포지션 출력	
	printf("--------------------------------\n");

	fclose(fp);

	return 0;
}

int main(int argc, char **argv)
{
	create_file();
	read_file();

	return 0;
}

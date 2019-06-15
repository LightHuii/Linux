#include<stdio.h>
#include<memory.h>

int write_to_file(){
	FILE *fp;
	fp = fopen("data", "w");
	if(fp == NULL){
		printf("fopen error\n");
		return -1;
	}

	fputs("hellow world\n", fp);

	fclose(fp);

	return 0;	
}

int read_from_file(){	
	FILE *fp;
	char buf[1024];

	fp = fopen("data", "r");
	if(fp == NULL){
		printf("fopen error\n");
		return -1;
	}

	memset(buf, 0, sizeof(buf));
	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	printf("%s", buf);

	return 0;
}

int main(int argc, char *argv[]){
	if(write_to_file()){
		printf("write to file\n");
		return -1;
	}

	if(read_from_file()){
		printf("read to file\n");
		return -1;
	}
	return 0;
}

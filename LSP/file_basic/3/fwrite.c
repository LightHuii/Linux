#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct person{
	char name[20];
	int age;
};

static int write_to_file(){
	FILE *fp = NULL;
	struct person kim = {
		.name = "kim",
		.age = 20
	};
		
	struct person lee = {
		.name = "lee",
		.age = 30
	};
	
	if(!(fp = fopen("persons", "w"))){
		return -1;
	}

	if(fwrite(&kim, sizeof(struct person), 1, fp) != 1){		//파일에 내용 저장: kim에 대한 내용
		goto err;
	}

	if(fwrite(&lee, sizeof(struct person), 1, fp) != 1){		//파일에 내용 저장: lee 내용
		goto err;
	}
	fclose(fp);
	return 0;

err:
	//만약 파일이 열려있다면 닫고, 실패 리턴
	if(fp){
		fclose(fp);
	}
	return -1;
}

static int read_from_file()
{	
	FILE *fp;
	struct person persons[2];

	if(!(fp = fopen("persons", "r"))){
		return -1;
	}
	
	if(fread(persons, sizeof(struct person), 2, fp) != 2){	//persons라는 person 정보를 2개 읽음
		fclose(fp);
		return -1;
	}

	fclose(fp);
	
	//읽어온 데이터 출력
	for(int i=0; i<2; i++){
		printf("name: %s, age: %d\n", persons[i].name, persons[i].age);
	}
	
	return 0;
}

int main(int argc, char **argv)
{
	if(write_to_file()){
		printf("write fail\n");
		return -1;
	}
	
	if(read_from_file()){
		printf("read fail\n");
		return -1;
	}
	return 0;
}

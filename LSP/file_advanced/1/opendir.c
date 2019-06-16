#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

//struct dirent *entry의 매크로를 문자열로 변환
#define TYPE2STR(X) \
	((X) == DT_BLK		? "block device" :\
	 (X) == DT_CHR		? "char device" :\
	 (X) == DT_DIR 		? "directory" :\
	 (X) == DT_FIFO		? "fifo" :\
	 (X) == DT_LNK 		? "symlink" :\
	 (X) == DT_REG 		? "regular file" :\
	 (X) == DT_SOCK		? "socket" :\
	 "unkown")

int main(int argc, char **argv)
{
	DIR *dp;
	struct dirent *entry;
	
	dp = opendir(".");	//현재 디렉토리를 연다는 것을 의미
	if(!dp){
		printf("opendir() fail\n");
		return -1;
	}
	
	while(entry = readdir(dp)){	//디렉터리의 파일을 끝날 때까지 열기
		printf("%s: %s\n",
				entry->d_name,
				TYPE2STR(entry->d_type));
	}
	closedir(dp);	//디렉터리 닫기
	return 0;
}



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/inotify.h>

#if 0
struct inotify_event {
    int      wd;        //Watch descriptor
    uint32_t mask;      /Mask describing event 
    uint32_t cookie;    //Unique cookie associating related
							events (for rename(2)) 
    uint32_t len;       //Size of name field 
    char     name[];    //Optional null-terminated name 
};
#endif


int main(int argc, char **argv)
{
	int fd = -1;	//inotify 객체의 파일 디스크립터
	int wd1 = -1;	//watch1 디스크립터
	int wd2 = -1;	//watch2 디스크립터
	int ret;		//리턴 값
	char buf[1024];
	struct inotify_event *event;	//inotify 객체의 이벤트 정보

	fd = inotify_init();	//inotify 객체 리턴
	if(fd == -1){
		printf("inotify_init() fail\n");
		return -1;
	}

	wd1 = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);	//fd, 감시할 경로, flag 
	if(wd1 == -1){
		printf("add_watch1() fail\n");
		goto err;
	}
	wd2 = inotify_add_watch(fd, "../../file_basic", IN_CREATE | IN_DELETE);	//fd, 감시할 경로, flag 
	if(wd2 == -1){
		printf("add_watch2() fail\n");
		goto err;
	}

	while(1){
		ret = read(fd, buf, sizeof(buf));	//buf에 inotify 이벤트 watch 정보 읽어 오기
		//ret는 읽은 buf의 바이트 수: event 정보의 총 바이트
		//기본적으로 blocking 상태로, 이벤트가 발생할 때까지 대기한다.
		if(ret == -1){
			printf("read() fail\n");
			break;
		}
	
		//이벤트 발생
		event = (struct inotify_event *)&buf[0];	//event 정보 입력. event가 포인터이므로 포인터로 받는다.
		while (ret > 0){	//현재 읽어온 event 정보가 끝날 때까지
		  	if(event->mask & IN_CREATE){		//파일이 생성될 때
		   		printf("file %s created\n", event->name);
		   	}
		   	if(event->mask & IN_DELETE){		//파일이 삭제될 때
		   		printf("file %s deleted\n", event->name);
		   	}
			event = (struct inotify_event*)((char *)event + sizeof(struct inotify_event) + event->len);
			//다음 inotify_event 정보 포인터
			ret -= sizeof(struct inotify_event) + event->len;
			//읽은 inotify_event 구조체 크기만큼 바이트 수를 빼준다.
		}
	}
	close(wd1);
	close(wd2);
	close(fd);
	
	return 0;

err:
	if(fd >= 0){
		close(fd);
	}
	if(wd1 >= 0){
		close(wd1);
	}
	if(wd2 >= 0){
        close(wd2);
    }

	return -1;
}

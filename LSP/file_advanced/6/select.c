#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	int fd = -1;	//inotify 객체의 파일 디스크립터
	int wd1 = -1;	//watch1 디스크립터
	int wd2 = -1;	//watch2 디스크립터
	int ret;		//리턴 값
	char buf[1024];
	struct inotify_event *event;	//inotify 객체의 이벤트 정보
	fd_set fds;		//fd set
	struct timeval timeout;	//timeout 구조체


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
		//select를 호출하기 전에 fds를 설정해 주어야 한다.
		FD_ZERO(&fds);		//fd set 초기화
		FD_SET(fd, &fds);	//inotify fd를 fds에 등록
		FD_SET(STDIN_FILENO, &fds);	//stdin fd를 fds에 등록	
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;	//5초로 설정

		ret = select(fd > STDIN_FILENO ? fd + 1 : STDIN_FILENO + 1,
					 &fds, NULL, NULL, &timeout);
		//fd 중 가장 큰 값 + 1, readfds, writefds, exceptfds, timeout
		
		if(ret == -1) {		//에러 발생
			goto err;
		} else if(ret == 0) {	//타임 아웃
			printf("select() timeout occur!\n");
		} else if(ret > 0) {	//이벤트 발생
			//inotify의 이벤트일 경우
			if(FD_ISSET(fd, &fds)){		
				ret = read(fd, buf, sizeof(buf));	//fd 읽어오기
				if(ret == -1){
					printf("read() fail\n");
					break;
				}

				event = (struct inotify_event *)&buf[0];
				while (ret > 0){
				  	if(event->mask & IN_CREATE){		//파일이 생성될 때
				   		printf("file %s created\n", event->name);
				   	}
				   	if(event->mask & IN_DELETE){		//파일이 삭제될 때
				   		printf("file %s deleted\n", event->name);
				   	}
					event = (struct inotify_event*)((char *)event + sizeof(struct inotify_event) + event->len);
					ret -= sizeof(struct inotify_event) + event->len;
				}
			} 
			//stdin 이벤트일 경우
			else if(FD_ISSET(STDIN_FILENO, &fds)){
				memset(buf, 0, sizeof(buf));
				ret = read(STDIN_FILENO, buf, sizeof(buf));		//STDIN fd 읽기
				if(ret == -1){
					printf("read() fail\n");
					break;
				}
				printf("user input [%s]\n", buf);	//stdin 입력 문자열 출력
			}
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

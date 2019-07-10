#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>

int main(int argc, char **argv)
{
	int fd = -1;	//inotify 객체의 파일 디스크립터
	int wd1 = -1;	//watch1 디스크립터
	int wd2 = -1;	//watch2 디스크립터
	int ret;		//리턴 값
	char buf[1024];
	struct inotify_event *event;	//inotify 객체의 이벤트 정보
	int epfd;
	struct epoll_event ep_event;

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

	//epoll 초기화
	epfd = epoll_create1(0);
	if (epfd == -1){
		printf("epoll_create() fail\n");
		goto err;
	}
	
	//fd와 이벤트를 epoll instance에 설정
	memset(&ep_event, 0, sizeof(ep_event));
	ep_event.events = EPOLLIN;		//EPOLLIN에 대해 감시
	ep_event.data.fd = fd;			//fd를 등록
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ep_event) == -1){
		printf("epoll_ctl() fail\n");
		goto err;
	}

	memset(&ep_event, 0, sizeof(ep_event));
	ep_event.events = EPOLLIN;		//EPOLLIN에 대해 감시
	ep_event.data.fd = STDIN_FILENO;			//fd를 등록
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ep_event) == -1){
		printf("epoll_ctl() fail\n");
		goto err;
	}

	while(1){
		memset(&ep_event, 0, sizeof(ep_event));
		ret = epoll_wait(epfd, &ep_event, 1, 5000);	//하나씩 읽기, 5초

		if(ret == -1) {		//에러 발생
			goto err;
		} else if(ret == 0) {	//타임 아웃
			printf("epoll wait() timeout occur!\n");
		} else if(ret > 0) {	//이벤트 발생
			//inotify의 이벤트일 경우
			if(ep_event.data.fd == fd){
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
			else if(ep_event.data.fd == STDIN_FILENO){
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
	close(epfd);
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
	if(epfd >= 0){
        close(wd2);
    }
	return -1;
}

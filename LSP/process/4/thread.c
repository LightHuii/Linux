#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#define MAX_TRY 100000000
int shared = 0;			//스레드들의 공유 자원(data 영역)
pthread_mutex_t mutex;	//mutex 인스턴스

//생성된 스레드가 시작할 함수
void *child_thread_main(void *arg){
	unsigned int i;
	printf("child pthread id: %lu\n", pthread_self());
	
	//임의로 공유 자원을 변화
	pthread_mutex_lock(&mutex);		//lock
	for(i = 0; i < MAX_TRY; i++){
		shared++;
	}
	pthread_mutex_unlock(&mutex);	//unlock
	
	pthread_mutex_lock(&mutex);		//lock
	for(i = 0; i < MAX_TRY; i++){
		shared--;
	}
	pthread_mutex_unlock(&mutex);	//unlock
	
	return NULL;
}

int main(int argc, char **argv)
{
	int ret;
	pthread_t child_threads[2];		//2개의 자식 스레드
	
	//뮤텍스 초기화
	if(pthread_mutex_init(&mutex, NULL)){
		printf("mutex_init() fail\n");
	}

	//메인 스레드 id
	printf("main pthread id: %ld\n", pthread_self());

	//첫 번째 자식 스레드 생성, child_thread_main이라는 함수부터 실행 시작
	ret = pthread_create(&child_threads[0], NULL, child_thread_main, NULL);
	if(ret){	//생성 실패시
		printf("pthread_create() fail(%d)\n", ret);
		return -1;
	}

	//두 번째 자식 스레드 생성, child_thread_main이라는 함수부터 실행 시작
	ret = pthread_create(&child_threads[1], NULL, child_thread_main, NULL);
	if(ret){	//생성 실패시
		printf("pthread_create() fail(%d)\n", ret);
		return -1;
	}
	
	//첫 번째 스레드를 종료 요청하고 기다림
	printf("trying to join: %lu\n", child_threads[0]);
	if(pthread_join(child_threads[0], NULL)){
		printf("pthread_join(%lu) fial\n", child_threads[0]);
	}

	//두 번째 스레드를 종료 요청하고 기다림
	printf("trying to join: %lu\n", child_threads[1]);
	if(pthread_join(child_threads[1], NULL)){
		printf("pthread_join(%lu) fial\n", child_threads[1]);
	}
	
	//공유된 자원 결과 출력
	printf("shared = %d\n", shared);
	printf("Done\n");

	return 0;
}

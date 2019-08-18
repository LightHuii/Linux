#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

#define MAX_TRY 1000000
int shared = 0;
pthread_mutex_t mutex;

void *child_thread_main(void *arg){
	unsigned int i;
	printf("child pthread id %lu\n", pthread_self());
	
	//shared에 접근할 때마다 lock을 걸어 다른 스레드가 접근하지 못 하도록
	for(i = 0; i< MAX_TRY; i++){	
		pthread_mutex_lock(&mutex);	
		shared++;
		pthread_mutex_unlock(&mutex);	
	}
	
	for(i = 0; i< MAX_TRY; i++){
		pthread_mutex_lock(&mutex);	
		shared--;
		pthread_mutex_unlock(&mutex);	
	}

	return NULL;
}

int main(int argc, char **argv)
{
	int ret;
	pthread_t child_threads[2];
	if(pthread_mutex_init(&mutex, NULL)){
		printf("mutex_init() fail\n");
	}

	printf("main pthread id %lu\n", pthread_self());
	
	//스레드 생성0
	ret = pthread_create(&child_threads[0], NULL, child_thread_main, NULL);
	if(ret){
		printf("pthread_create() fail(%d)\n", ret);
		return -1;
	}

	//스레드 생성1
	ret = pthread_create(&child_threads[1], NULL, child_thread_main, NULL);
	if(ret){
		printf("pthread_create() fail(%d)\n", ret);
		return -1;
	}

	//join하여 스레드 제거 - 바로 join되진 않음
	printf("trying to join %lu\n", child_threads[0]);
	if(pthread_join(child_threads[0], NULL)){
		printf("pthread_join(%lu) fail\n", child_threads[0]);
	}
	//join
	printf("trying to join %lu\n", child_threads[1]);
	if(pthread_join(child_threads[1], NULL)){
		printf("pthread_join(%lu) fail\n", child_threads[1]);
	}

	printf("shared = %d\n", shared);
	printf("Done.\n");
	return 0;
}

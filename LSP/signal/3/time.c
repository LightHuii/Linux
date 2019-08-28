#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
#include<sys/time.h>

#if 0
struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-기59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };
#endif

static void print_cur_time(){
	time_t now;
	struct tm *now_tm;

	now = time(NULL);		//UTC 기준 시간
	printf("current: %ld, ", now);

	now_tm = localtime(&now);	//현재 시간(정형화)
	if(!now_tm) {
		printf("localtime() fail\n");
		return;
	}
	printf("%d/%d/%d %d-%d-%d\n",
			now_tm->tm_year + 1900,
			now_tm->tm_mon + 1,
			now_tm->tm_mday,
			now_tm->tm_hour,
			now_tm->tm_min,
			now_tm->tm_sec);
}

//알람 시그널 시작 함수
static void sigalarm_handler(int signum){
	printf("got SIGALARM\n");
	print_cur_time();
}


#if 0
           struct itimerval {
               struct timeval it_interval; /* Interval for periodic timer */
               struct timeval it_value;    /* Time until next expiration */
           };

           struct timeval {
               time_t      tv_sec;         /* seconds */
               suseconds_t tv_usec;        /* microseconds */
           };

#endif

int main(int argc, char **argv)
{
	struct itimerval ival;

	signal(SIGALRM, sigalarm_handler);
	
	print_cur_time();	//우선 현재 시간 출력
	//alarm(5);			//5초 알람 시그널 설정

	ival.it_value.tv_sec = 5;
	ival.it_value.tv_usec = 0;		//처음엔 5초
	ival.it_interval.tv_sec = 1;
	ival.it_interval.tv_usec = 0;		//그 후 1초

	if(setitimer(ITIMER_REAL, &ival, NULL) != 0){
		printf("setitimer() error\n");
	}

	while(1){			//1초씩 sleep하며 대
		sleep(1);
	}

	return 0;
}

#include <sll/api/date.h>
#include <sll/api/time.h>
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>



static sll_time_zone_t _linux_platform_time_zone={"GMT",0};



__SLL_EXTERNAL const sll_time_zone_t* sll_platform_time_zone=&_linux_platform_time_zone;



extern int daylight;
extern long timezone;
extern char* tzname[2];



void _deinit_platform_time(void){
	_linux_platform_time_zone=*sll_utc_time_zone;
}



void _init_platform_time(void){
	tzset();
	const sll_char_t* nm=SLL_CHAR(tzname[!!daylight]);
	sll_string_length_t sz=sll_string_length(nm);
	if (sz>31){
		sz=31;
	}
	sll_copy_data(nm,sz,_linux_platform_time_zone.name);
	_linux_platform_time_zone.name[sz]=0;
	_linux_platform_time_zone.offset=-timezone/60;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	sll_time_t e=ts.tv_sec*1000000000+ts.tv_nsec+tm;
	while (1){
		struct timeval tv;
		tv.tv_sec=tm/1000000000;
		tv.tv_usec=(tm/1000)%1000000;
		if (!select(0,NULL,NULL,NULL,&tv)){
			return;
		}
		clock_gettime(CLOCK_REALTIME,&ts);
		sll_time_t c=ts.tv_sec*1000000000+ts.tv_nsec;
		if (c>=e){
			break;
		}
		tm=c-e;
	}
}

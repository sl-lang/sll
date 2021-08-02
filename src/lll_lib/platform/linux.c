#include <lll/common.h>
#include <lll/types.h>
#include <time.h>



__LLL_FUNC lll_time_t lll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__LLL_FUNC void lll_platform_setup_console(void){
}

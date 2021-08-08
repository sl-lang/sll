#include <lll/common.h>
#include <lll/platform.h>
#include <lll/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



const char* lll_platform_string="posix";



__LLL_FUNC void* lll_platform_allocate_page(lll_page_size_t sz){
	return mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
}



__LLL_FUNC void lll_platform_free_page(void* pg,lll_page_size_t sz){
	munmap(pg,sz);
}



__LLL_FUNC lll_time_t lll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__LLL_FUNC lll_time_t lll_platform_get_page_size(void){
	return sysconf(_SC_PAGESIZE);
}



__LLL_FUNC lll_buffer_size_t lll_platform_path_absolute(const char* fp,lll_buffer_t bf,lll_buffer_size_t bfl){
	return !!realpath(fp,(char*)bf);
}



__LLL_FUNC lll_bool_t lll_platform_path_exists(const char* fp){
	return !access(fp,F_OK);
}



__LLL_FUNC lll_bool_t lll_platform_path_is_directory(const char* fp){
	struct stat st;
	lll_bool_t o=0;
	if (!stat(fp,&st)&&(st.st_mode&S_IFDIR)){
		o=1;
	}
	return o;
}



__LLL_FUNC void lll_platform_setup_console(void){
}

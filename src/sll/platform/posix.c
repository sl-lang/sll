#include <sll/common.h>
#include <sll/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>



const char* sll_platform_string="posix";



__SLL_FUNC void* sll_platform_allocate_page(sll_page_size_t sz){
	return mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
}



__SLL_FUNC void sll_platform_free_page(void* pg,sll_page_size_t sz){
	munmap(pg,sz);
}



__SLL_FUNC sll_time_t sll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__SLL_FUNC sll_time_t sll_platform_get_page_size(void){
	return sysconf(_SC_PAGESIZE);
}



__SLL_FUNC sll_buffer_size_t sll_platform_path_absolute(const char* fp,sll_buffer_t bf,sll_buffer_size_t bfl){
	return !!realpath(fp,(char*)bf);
}



__SLL_FUNC sll_bool_t sll_platform_path_exists(const char* fp){
	return !access(fp,F_OK);
}



__SLL_FUNC sll_bool_t sll_platform_path_is_directory(const char* fp){
	struct stat st;
	sll_bool_t o=0;
	if (!stat(fp,&st)&&(st.st_mode&S_IFDIR)){
		o=1;
	}
	return o;
}

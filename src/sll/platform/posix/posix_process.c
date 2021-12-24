#include <sll/common.h>
#include <sll/types.h>
#include <unistd.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void){
	return getpid();
}

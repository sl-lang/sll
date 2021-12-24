#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/types.h>



__API_FUNC(process_get_pid){
	return sll_platform_get_pid();
}

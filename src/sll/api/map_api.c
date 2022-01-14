#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/types.h>



__API_FUNC(map_extend){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(map_remove){
	SLL_RELEASE(sll_map_remove(NULL,b,a));
}

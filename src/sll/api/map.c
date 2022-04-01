#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/types.h>



__API_FUNC(map_extend){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(map_remove){
	GC_RELEASE(sll_map_remove(NULL,b,a));
}

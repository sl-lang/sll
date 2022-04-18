#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL void sll_api_map_extend(sll_map_t* map,const sll_map_t* new){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_map_remove(sll_map_t* map,sll_object_t* key){
	GC_RELEASE(sll_map_remove(NULL,key,map));
}

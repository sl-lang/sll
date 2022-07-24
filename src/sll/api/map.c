#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL void sll_api_map_extend(sll_map_t* map,const sll_map_t* new){
	sll_map_join(NULL,new,map);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_map_length_t sll_api_map_remove(sll_map_t* map,sll_object_t key){
	SLL_RELEASE(sll_map_remove(NULL,key,map));
	return map->length;
}

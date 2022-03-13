#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/internal/api.h>
#include <sll/internal/common.h>
#include <sll/map.h>
#include <sll/types.h>



__API_FUNC(map_extend){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(map_remove){
	sll_release_object(sll_map_remove(NULL,b,a));
}

#ifndef __SLL_MAP_H__
#define __SLL_MAP_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_ZERO_MAP(m) \
	do{ \
		(m)->l=0; \
		(m)->v=NULL; \
	} while (0)
#define SLL_ZERO_MAP_STRUCT {0,NULL}



__SLL_FUNC void sll_map_add(const sll_map_t* m,sll_runtime_object_t* k,sll_runtime_object_t* v,sll_map_t* o);



__SLL_FUNC void sll_map_add_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o);



__SLL_FUNC void sll_map_add_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o);



__SLL_FUNC void sll_map_clone(const sll_map_t* m,sll_map_t* o);



__SLL_FUNC void sll_map_create(sll_map_length_t l,sll_map_t* o);



__SLL_FUNC void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* o);



__SLL_FUNC void sll_map_keys(const sll_map_t* m,sll_array_t* o);



__SLL_FUNC sll_runtime_object_t* sll_map_remove(const sll_map_t* m,sll_runtime_object_t* k,sll_map_t* o);



__SLL_FUNC void sll_map_values(const sll_map_t* m,sll_array_t* o);



#endif

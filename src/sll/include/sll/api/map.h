#ifndef __SLL_API_MAP_H__
#define __SLL_API_MAP_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Map API
 * \group map-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_map_extend
 * \group map-api
 * \desc Docs!
 * \api m#m|v
 * \arg sll_map_t* map
 * \arg const sll_map_t* new
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_map_extend(sll_map_t* map,const sll_map_t* new);



/**
 * \flags func
 * \name sll_api_map_remove
 * \group map-api
 * \desc Docs!
 * \api mo|v
 * \arg sll_map_t* map
 * \arg sll_object_t* key
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_map_remove(sll_map_t* map,sll_object_t* key);



#endif

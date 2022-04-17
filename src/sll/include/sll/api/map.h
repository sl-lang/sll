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
 * \api mm|v
 * \arg sll_map_t* a
 * \arg sll_map_t* b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_map_extend(sll_map_t* a,sll_map_t* b);



/**
 * \flags func
 * \name sll_api_map_remove
 * \group map-api
 * \desc Docs!
 * \api mo|v
 * \arg sll_map_t* a
 * \arg sll_object_t* b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_map_remove(sll_map_t* a,sll_object_t* b);



#endif

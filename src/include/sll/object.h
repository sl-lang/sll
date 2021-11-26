#ifndef __SLL_OBJECT_H__
#define __SLL_OBJECT_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Object
 * \group object
 * \desc Docs!
 */



#define SLL_OBJECT_TYPE_INT 0
#define SLL_OBJECT_TYPE_FLOAT 1
#define SLL_OBJECT_TYPE_CHAR 2
#define SLL_OBJECT_TYPE_STRING 3
#define SLL_OBJECT_TYPE_ARRAY 4
#define SLL_OBJECT_TYPE_HANDLE 5
#define SLL_OBJECT_TYPE_MAP 6
#define SLL_OBJECT_TYPE_MAP_KEYS 7
#define SLL_OBJECT_TYPE_MAP_VALUES 8
#define SLL_OBJECT_TYPE_RESERVED2 62
#define SLL_OBJECT_TYPE_RESERVED3 63

#define SLL_MAX_RUNTIME_OBEJCT_TYPE SLL_OBJECT_TYPE_MAP_VALUES

#define SLL_OBJECT_FLAG_RESERVED0 64
#define SLL_OBJECT_FLAG_RESERVED1 128

#define SLL_OBJECT_GET_TYPE(r) ((r)->t&0x3f)
#define SLL_OBJECT_IS_STORAGE(r) (SLL_OBJECT_GET_TYPE((r))==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE((r))==SLL_OBJECT_TYPE_ARRAY||SLL_OBJECT_GET_TYPE((r))==SLL_OBJECT_TYPE_MAP)



/**
 * \flags check_output func
 * \name sll_object_get_type
 * \group object
 * \desc Docs!
 * \arg const sll_object_t* o
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(const sll_object_t* o);



#endif

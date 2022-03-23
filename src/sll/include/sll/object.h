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
/**
 * \flags subgroup
 * \name Types
 * \group object
 * \subgroup object-type
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_INT
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_INT 0



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_FLOAT
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_FLOAT 1



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_CHAR
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_CHAR 2



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_STRING
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_STRING 3



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_ARRAY
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_ARRAY 4



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_MAP
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_MAP 5



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_MAP_KEYS
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_MAP_KEYS 6



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_MAP_VALUES
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_MAP_VALUES 7



/**
 * \flags macro var
 * \name SLL_OBJECT_TYPE_OBJECT
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_TYPE_OBJECT 0xffffff



/**
 * \flags macro var
 * \name SLL_MAX_OBJECT_TYPE
 * \group object
 * \subgroup object-type
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_MAX_OBJECT_TYPE SLL_OBJECT_TYPE_MAP_VALUES



/**
 * \flags macro var
 * \name SLL_OBJECT_FLAG_CONSTANT
 * \group object
 * \desc Docs!
 * \type sll_object_type_t
 */
#define SLL_OBJECT_FLAG_CONSTANT 0x80000000



/**
 * \flags func macro
 * \name SLL_OBJECT_GET_TYPE
 * \group object
 * \desc Docs!
 * \arg sll_object_t* r
 * \ret sll_object_type_t
 */
#define SLL_OBJECT_GET_TYPE(r) ((r)->t&0xffffff)



/**
 * \flags func macro
 * \name SLL_OBJECT_GET_TYPE_MASK
 * \group object
 * \desc Docs!
 * \arg sll_object_type_t t
 * \ret sll_object_type_t
 */
#define SLL_OBJECT_GET_TYPE_MASK(t) ((t)&0xffffff)



/**
 * \flags func macro
 * \name SLL_OBJECT_IS_STORAGE
 * \group object
 * \desc Docs!
 * \arg sll_object_t* r
 * \ret sll_bool_t
 */
#define SLL_OBJECT_IS_STORAGE(r) (SLL_OBJECT_GET_TYPE((r))>=SLL_OBJECT_TYPE_STRING&&SLL_OBJECT_GET_TYPE((r))<=SLL_OBJECT_TYPE_MAP_VALUES)



/**
 * \flags func macro
 * \name SLL_INIT_OBJECT_TYPE_TABLE
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* o
 */
#define SLL_INIT_OBJECT_TYPE_TABLE(o) \
    do{ \
        (o)->dt=NULL; \
        (o)->l=0; \
    } while (0)



/**
 * \flags macro var
 * \name SLL_INIT_OBJECT_TYPE_TABLE_STRUCT
 * \group object
 * \desc Docs!
 * \type sll_object_type_table_t
 */
#define SLL_INIT_OBJECT_TYPE_TABLE_STRUCT {NULL,0}



/**
 * \flags check_output func
 * \name sll_add_type
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* tt
 * \arg sll_object_t*const* p
 * \arg sll_arg_count_t l
 * \arg const sll_string_t* nm
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* tt,sll_object_t*const* p,sll_arg_count_t l,const sll_string_t* nm);



/**
 * \flags check_output func
 * \name sll_create_new_object_type
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* tt
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_new_object_type(sll_object_type_table_t* tt);



/**
 * \flags check_output func
 * \name sll_create_object_type
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg sll_object_type_t t
 * \arg sll_object_t*const* p
 * \arg sll_arg_count_t l
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object_type(const sll_object_type_table_t* tt,sll_object_type_t t,sll_object_t*const* p,sll_arg_count_t l);



/**
 * \flags func
 * \name sll_free_object_type_list
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* tt
 */
__SLL_EXTERNAL void sll_free_object_type_list(sll_object_type_table_t* tt);



/**
 * \flags func
 * \name sll_get_type_name
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg sll_object_type_t t
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_get_type_name(sll_object_type_table_t* tt,sll_object_type_t t,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_object_clone
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg sll_object_t* o
 * \arg sll_bool_t d
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_clone(const sll_object_type_table_t* tt,sll_object_t* o,sll_bool_t d);



/**
 * \flags check_output func
 * \name sll_object_get_field
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg sll_object_t* o
 * \arg const sll_string_t* f
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_get_field(const sll_object_type_table_t* tt,sll_object_t* o,const sll_string_t* f);



/**
 * \flags check_output func
 * \name sll_object_get_type
 * \group object
 * \desc Docs!
 * \arg sll_object_t* o
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(sll_object_t* o);



/**
 * \flags func
 * \name sll_object_set_field
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg sll_object_t* o
 * \arg const sll_string_t* f
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_object_set_field(const sll_object_type_table_t* tt,sll_object_t* o,const sll_string_t* f,sll_object_t* v);



/**
 * \flags func
 * \name sll_object_to_array
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg sll_object_t* o
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL void sll_object_to_array(const sll_object_type_table_t* tt,sll_object_t* o,sll_array_t* out);



#endif

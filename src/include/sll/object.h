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
#define SLL_OBJECT_TYPE_RESERVED0 0x1ffffffe
#define SLL_OBJECT_TYPE_RESERVED1 0x1fffffff

#define SLL_MAX_OBJECT_TYPE SLL_OBJECT_TYPE_MAP_VALUES

#define SLL_OBJECT_FLAG_CONSTANT 0x20000000
#define SLL_OBJECT_FLAG_RESERVED0 0x40000000
#define SLL_OBJECT_FLAG_RESERVED1 0x80000000

#define SLL_OBJECT_GET_TYPE(r) ((r)->t&0x1fffffff)
#define SLL_OBJECT_GET_TYPE_MASK(t) ((t)&0x1fffffff)
#define SLL_OBJECT_IS_STORAGE(r) (SLL_OBJECT_GET_TYPE((r))==SLL_OBJECT_TYPE_STRING||SLL_OBJECT_GET_TYPE((r))==SLL_OBJECT_TYPE_ARRAY||SLL_OBJECT_GET_TYPE((r))==SLL_OBJECT_TYPE_MAP)

#define SLL_INIT_OBJECT_TYPE_TABLE(o) \
    do{ \
        (o)->dt=NULL; \
        (o)->l=0; \
    } while (0)
#define SLL_INIT_OBJECT_TYPE_TABLE_STRUCT {NULL,0}



/**
 * \flags check_output func
 * \name sll_add_type
 * \group object
 * \desc Docs!
 * \arg sll_object_type_initializer_table_t* ot_it
 * \arg const sll_object_type_field_t* p
 * \arg sll_arg_count_t l
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_initializer(sll_object_type_initializer_table_t* ot_it,const sll_object_type_field_t* p,sll_arg_count_t l);



/**
 * \flags check_output func
 * \name sll_add_type
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* tt
 * \arg const sll_object_t*const* p
 * \arg sll_arg_count_t l
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_add_type(sll_object_type_table_t* tt,const sll_object_t*const* p,sll_arg_count_t l);



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
 * \flags check_output func
 * \name sll_object_clone
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg const sll_object_t* o
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_clone(const sll_object_type_table_t* tt,const sll_object_t* o);



/**
 * \flags check_output func
 * \name sll_object_get_field
 * \group object
 * \desc Docs!
 * \arg const sll_object_type_table_t* tt
 * \arg const sll_object_t* o
 * \arg const sll_string_t* f
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_object_get_field(const sll_object_type_table_t* tt,const sll_object_t* o,const sll_string_t* f);



/**
 * \flags check_output func
 * \name sll_object_get_type
 * \group object
 * \desc Docs!
 * \arg const sll_object_t* o
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_object_get_type(const sll_object_t* o);



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
 * \flags check_output func
 * \name sll_type_from_initializer
 * \group object
 * \desc Docs!
 * \arg sll_object_type_table_t* tt
 * \arg const sll_string_table_t* st
 * \arg const sll_object_type_initializer_t* oi
 * \ret sll_object_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_type_t sll_type_from_initializer(sll_object_type_table_t* tt,const sll_string_table_t* st,const sll_object_type_initializer_t* oi);



#endif

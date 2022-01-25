#ifndef __SLL_STATIC_OBJECT_H__
#define __SLL_STATIC_OBJECT_H__ 1
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Static Object
 * \group static-object
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Creation
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_ACQUIRE_STATIC
 * \group static-object
 * \desc Docs!
 * \arg <C identifier> nm
 * \ret sll_object_t*
 */
#define SLL_ACQUIRE_STATIC(nm) (SLL_ACQUIRE(sll_static_##nm),sll_static_##nm)



/**
 * \flags func macro
 * \name SLL_ACQUIRE_STATIC_INT
 * \group static-object
 * \desc Docs!
 * \arg sll_integer_t v
 * \ret sll_object_t*
 */
#define SLL_ACQUIRE_STATIC_INT(v) (SLL_ACQUIRE(((v)<0?sll_static_negative_int[-(v)-1]:sll_static_int[(v)])),((v)<0?sll_static_negative_int[-(v)-1]:sll_static_int[(v)]))



/**
 * \flags func macro
 * \name SLL_ACQUIRE_STATIC_CHAR
 * \group static-object
 * \desc Docs!
 * \arg sll_char_t c
 * \ret sll_object_t*
 */
#define SLL_ACQUIRE_STATIC_CHAR(c) SLL_FROM_CHAR(c)



/**
 * \flags func macro
 * \name SLL_FROM_INT
 * \group static-object
 * \desc Docs!
 * \arg sll_integer_t v
 * \ret sll_object_t*
 */
#define SLL_FROM_INT(v) __SLL_ADD_DEBUG_DATA(sll_int_to_object(v),__SLL_DEBUG_TYPE_CREATE)



/**
 * \flags func macro
 * \name SLL_FROM_FLOAT
 * \group static-object
 * \desc Docs!
 * \arg sll_float_t v
 * \ret sll_object_t*
 */
#define SLL_FROM_FLOAT(v) __SLL_ADD_DEBUG_DATA(sll_float_to_object(v),__SLL_DEBUG_TYPE_CREATE)



/**
 * \flags func macro
 * \name SLL_FROM_CHAR
 * \group static-object
 * \desc Docs!
 * \arg sll_char_t c
 * \ret sll_object_t*
 */
#define SLL_FROM_CHAR(c) (__SLL_ADD_DEBUG_DATA(sll_static_char[(sll_char_t)(c)],__SLL_DEBUG_TYPE_CREATE)->rc++,sll_static_char[(sll_char_t)(c)])



/**
 * \flags var
 * \name sll_static_int
 * \group static-object
 * \desc Docs!
 * \type sll_object_t**
 */
__SLL_EXTERNAL extern sll_object_t* sll_static_int[256];



/**
 * \flags var
 * \name sll_static_negative_int
 * \group static-object
 * \desc Docs!
 * \type sll_object_t**
 */
__SLL_EXTERNAL extern sll_object_t* sll_static_negative_int[16];



/**
 * \flags var
 * \name sll_static_float_zero
 * \group static-object
 * \desc Docs!
 * \type sll_object_t*
 */
__SLL_EXTERNAL extern sll_object_t* sll_static_float_zero;



/**
 * \flags var
 * \name sll_static_float_one
 * \group static-object
 * \desc Docs!
 * \type sll_object_t*
 */
__SLL_EXTERNAL extern sll_object_t* sll_static_float_one;



/**
 * \flags var
 * \name sll_static_float_half
 * \group static-object
 * \desc Docs!
 * \type sll_object_t*
 */
__SLL_EXTERNAL extern sll_object_t* sll_static_float_half;



/**
 * \flags var
 * \name sll_static_char
 * \group static-object
 * \desc Docs!
 * \type sll_object_t**
 */
__SLL_EXTERNAL extern sll_object_t* sll_static_char[256];



/**
 * \flags check_output func
 * \name sll_char_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_char_t v
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_char_to_object(sll_char_t v);



/**
 * \flags check_output func
 * \name sll_float_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_float_t v
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_float_to_object(sll_float_t v);



/**
 * \flags check_output func
 * \name sll_int_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_integer_t v
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_int_to_object(sll_integer_t v);



#endif

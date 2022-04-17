#ifndef __SLL_API_WEAKREF_H__
#define __SLL_API_WEAKREF_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Weak Reference API
 * \group weakref-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_weakref__init
 * \group weakref-api
 * \desc Docs!
 * \api oi|v
 * \arg sll_object_t* a
 * \arg sll_integer_t b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref__init(sll_object_t* a,sll_integer_t b);



/**
 * \flags func
 * \name sll_api_weakref_create
 * \group weakref-api
 * \desc Docs!
 * \api o|i
 * \arg sll_object_t* a
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_weakref_create(sll_object_t* a);



/**
 * \flags func
 * \name sll_api_weakref_delete
 * \group weakref-api
 * \desc Docs!
 * \api i|b
 * \arg sll_integer_t a
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_weakref_delete(sll_integer_t a);



/**
 * \flags func
 * \name sll_api_weakref_get
 * \group weakref-api
 * \desc Docs!
 * \api i|o
 * \arg sll_integer_t a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_weakref_get(sll_integer_t a);



/**
 * \flags func
 * \name sll_api_weakref_set_callback_data
 * \group weakref-api
 * \desc Docs!
 * \api io|v
 * \arg sll_integer_t a
 * \arg sll_object_t* b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref_set_callback_data(sll_integer_t a,sll_object_t* b);



#endif

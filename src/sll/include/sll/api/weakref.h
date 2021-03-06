#ifndef __SLL_API_WEAKREF_H__
#define __SLL_API_WEAKREF_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
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
 * \api oi|
 * \arg sll_object_t no_object
 * \arg sll_integer_t callback
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref__init(sll_object_t no_object,sll_integer_t callback);



/**
 * \flags check_output func
 * \name sll_api_weakref_create
 * \group weakref-api
 * \desc Docs!
 * \api o|Q
 * \arg sll_object_t object
 * \ret sll_weak_reference_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_api_weakref_create(sll_object_t object);



/**
 * \flags check_output func
 * \name sll_api_weakref_delete
 * \group weakref-api
 * \desc Docs!
 * \api Q|b
 * \arg sll_weak_reference_t weak_reference
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_weakref_delete(sll_weak_reference_t weak_reference);



/**
 * \flags check_output func
 * \name sll_api_weakref_get
 * \group weakref-api
 * \desc Docs!
 * \api Q|o
 * \arg sll_weak_reference_t weak_reference
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_weakref_get(sll_weak_reference_t weak_reference);



/**
 * \flags func
 * \name sll_api_weakref_set_callback_data
 * \group weakref-api
 * \desc Docs!
 * \api Qo|
 * \arg sll_weak_reference_t weak_reference
 * \arg sll_object_t callback
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref_set_callback_data(sll_weak_reference_t weak_reference,sll_object_t callback);



#endif

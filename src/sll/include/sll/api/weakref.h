#ifndef __SLL_API_WEAKREF_H__
#define __SLL_API_WEAKREF_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Weak Reference API
 * \group weakref-api
 * \desc Docs!
 */



/**
 * \flags api func
 * \name sll_api_weakref__init
 * \group weakref-api
 * \desc Docs!
 * \arg O
 * \arg I
 */
__API_FUNC_DECL(weakref__init);



/**
 * \flags api func
 * \name sll_api_weakref_create
 * \group weakref-api
 * \desc Docs!
 * \arg O
 * \ret I
 */
__API_FUNC_DECL(weakref_create);



/**
 * \flags api func
 * \name sll_api_weakref_delete
 * \group weakref-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(weakref_delete);



/**
 * \flags api func
 * \name sll_api_weakref_get
 * \group weakref-api
 * \desc Docs!
 * \arg I
 * \ret O
 */
__API_FUNC_DECL(weakref_get);



/**
 * \flags api func
 * \name sll_api_weakref_set_callback_data
 * \group weakref-api
 * \desc Docs!
 * \arg I
 * \arg O
 */
__API_FUNC_DECL(weakref_set_callback_data);



#endif

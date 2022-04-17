#ifndef __SLL_TLS_H__
#define __SLL_TLS_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Thread-Local Storage
 * \group tls
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_INIT_TLS
 * \group tls
 * \desc Docs!
 * \arg sll_tls_object_t* o
 */
#define SLL_INIT_TLS(o) \
	do{ \
		(o)->sz=0; \
		(o)->dt=NULL; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_INIT_TLS_STRUCT
 * \group tls
 * \desc Docs!
 * \type sll_tls_object_t
 */
#define SLL_INIT_ARRAY_STRUCT {0,NULL}



/**
 * \flags func
 * \name sll_free_tls
 * \group tls
 * \desc Docs!
 * \arg sll_tls_object_t* tls
 */
__SLL_EXTERNAL void sll_free_tls(sll_tls_object_t* tls);



/**
 * \flags check_output func
 * \name sll_tls_get
 * \group tls
 * \desc Docs!
 * \arg sll_tls_object_t* tls
 * \arg sll_object_t* out*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t** sll_tls_get(sll_tls_object_t* tls);



#endif

#ifndef __SLL_TLS_H__
#define __SLL_TLS_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/thread.h>
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
#define SLL_INIT_TLS_STRUCT {0,NULL}



/**
 * \flags type var
 * \name sll_tls_object_length_t
 * \group tls
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_tls_object_length_t;



/**
 * \flags type
 * \name sll_tls_value_t
 * \group tls
 * \desc Docs!
 * \arg sll_thread_index_t thread_index
 * \arg sll_object_t* value
 */
typedef struct _SLL_TLS_VALUE{
	sll_thread_index_t thread_index;
	sll_object_t* value;
} sll_tls_value_t;



/**
 * \flags type
 * \name sll_tls_object_t
 * \group tls
 * \desc Docs!
 * \arg sll_tls_object_length_t length
 * \arg sll_tls_value_t* data
 */
typedef struct _SLL_TLS_OBJECT{
	sll_tls_object_length_t length;
	sll_tls_value_t* data;
} sll_tls_object_t;



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
 * \ret sll_object_t**
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t** sll_tls_get(sll_tls_object_t* tls);



#endif

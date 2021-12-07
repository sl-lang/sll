#ifndef __SLL_HANDLE_H__
#define __SLL_HANDLE_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Handle
 * \group handle
 * \desc Docs!
 */



#define SLL_FROM_HANDLE(t,v) __SLL_ADD_DEBUG_DATA(sll_handle_to_object((t),(v)),__SLL_DEBUG_TYPE_CREATE)
#define SLL_HANDLE_DESCRIPTOR_HEADER(nm) nm,0,0
#define SLL_HANDLE_FREE 0xffffffffffffffffull
#define SLL_HANDLE_LOOKUP_DESCRIPTOR(hl,t) (*((hl)->dt+(t)-1))
#define SLL_HANDLE_UNKNOWN_TYPE 0
#define SLL_INIT_HANDLE_DATA(o) \
	do{ \
		(o)->t=SLL_HANDLE_UNKNOWN_TYPE; \
		(o)->h=0; \
	} while (0)
#define SLL_INIT_HANDLE_DATA_STRUCT {SLL_HANDLE_UNKNOWN_TYPE,0}
#define SLL_INIT_HANDLE_LIST_STRUCT {NULL,0}
#define SLL_MAX_HANDLE 0xfffffffffffffffeull



/**
 * \flags func
 * \name sll_cleanup_handles
 * \group handle
 * \desc Docs!
 * \arg sll_handle_list_t* hl
 * \arg sll_handle_type_t hll
 */
__SLL_EXTERNAL void sll_cleanup_handles(sll_handle_list_t* hl,sll_handle_type_t hll);



/**
 * \flags check_output func
 * \name sll_verify_object_stack_cleanup
 * \group handle
 * \desc Docs!
 * \arg sll_handle_list_t* hl
 * \arg sll_handle_descriptor_t* hd
 * \ret sll_handle_type_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_handle_type_t sll_create_handle(sll_handle_list_t* hl,sll_handle_descriptor_t* hd);



/**
 * \flags func
 * \name sll_free_handle_list
 * \group handle
 * \desc Docs!
 * \arg sll_handle_list_t* hl
 */
__SLL_EXTERNAL void sll_free_handle_list(sll_handle_list_t* hl);



/**
 * \flags check_output func
 * \name sll_handle_clone
 * \group handle
 * \desc Docs!
 * \arg sll_handle_type_t t
 * \arg sll_handle_t h
 * \ret sll_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_handle_t sll_handle_clone(sll_handle_type_t t,sll_handle_t h);



/**
 * \flags check_output func
 * \name sll_handle_to_object
 * \group static-object
 * \subgroup static-object-create
 * \desc Docs!
 * \arg sll_handle_type_t t
 * \arg sll_handle_t h
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_handle_to_object(sll_handle_type_t t,sll_handle_t h);



/**
 * \flags check_output func
 * \name sll_handle_lookup_descriptor
 * \group handle
 * \desc Docs!
 * \arg sll_handle_list_t* hl
 * \arg sll_handle_type_t t
 * \ret sll_handle_descriptor_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_handle_descriptor_t* sll_handle_lookup_descriptor(sll_handle_list_t* hl,sll_handle_type_t t);



#endif

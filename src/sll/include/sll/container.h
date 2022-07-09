#ifndef __SLL_CONTAINER_H__
#define __SLL_CONTAINER_H__ 1
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Container
 * \group container
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Handle Container
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Map Container
 * \group container
 * \subgroup container-map
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_CONTAINER_CLEAR
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 */
#define SLL_CONTAINER_CLEAR(c) \
	do{ \
		sll_container_t* __c=(c); \
		sll_allocator_release(__c->data); \
		__c->data=NULL; \
		__c->size=0; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_CONTAINER_FILTER
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg __type__ type
 * \arg __identifier__ var
 * \arg __code__ check
 * \arg __code__ delete
 */
#define SLL_CONTAINER_FILTER(c,type,var,check,delete) \
	do{ \
		sll_container_t* __c=(c); \
		sll_size_t __i=0; \
		type* __ptr=(type*)(__c->data); \
		for (sll_size_t __j=0;__j<__c->size;__j++){ \
			type var=*(__ptr+__j); \
			if (!(check)){ \
				delete; \
			} \
			else{ \
				*(__ptr+__i)=*(__ptr+__j); \
				__i++; \
			} \
		} \
		if (__i!=__c->size){ \
			__c->size=__i; \
			sll_allocator_resize(&(__c->data),__i*sizeof(type)); \
		} \
	} while (0)



/**
 * \flags func macro
 * \name SLL_CONTAINER_INIT
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 */
#define SLL_CONTAINER_INIT(c) \
	do{ \
		sll_container_t* __c=(c); \
		__c->data=NULL; \
		__c->size=0; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_CONTAINER_INIT_STRUCT
 * \group container
 * \desc Docs!
 * \type sll_container_t
 */
#define SLL_CONTAINER_INIT_STRUCT {NULL,0}



/**
 * \flags func macro
 * \name SLL_CONTAINER_ITER
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg __type__ type
 * \arg __identifier__ var
 * \arg __code__ code
 */
#define SLL_CONTAINER_ITER(c,type,var,code) \
	do{ \
		sll_container_t* __c=(c); \
		type* __ptr=(type*)(__c->data); \
		for (sll_size_t __i=0;__i<__c->size;__i++){ \
			type var=*(__ptr+__i); \
			code; \
		} \
	} while (0)



/**
 * \flags func macro
 * \name SLL_CONTAINER_ITER_CLEAR
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg __type__ type
 * \arg __identifier__ var
 * \arg __code__ code
 */
#define SLL_CONTAINER_ITER_CLEAR(c,type,var,code) \
	do{ \
		sll_container_t* __c_tmp=(c); \
		SLL_CONTAINER_ITER(__c_tmp,type,var,code); \
		SLL_CONTAINER_CLEAR(__c_tmp); \
	} while (0)



/**
 * \flags func macro
 * \name SLL_CONTAINER_PUSH
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg __type__ type
 * \arg __any__ elem
 */
#define SLL_CONTAINER_PUSH(c,type,elem) \
	do{ \
		sll_container_t* __c=(c); \
		__c->size++; \
		sll_allocator_resize(&(__c->data),__c->size*sizeof(type)); \
		*(((type*)(__c->data))+__c->size-1)=(elem); \
	} while (0)



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_ALLOC
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_size_t* idx
 */
#define SLL_HANDLE_CONTAINER_ALLOC(c,idx) \
	do{ \
		sll_handle_container_t* __c=(c); \
		sll_size_t* __idx=(idx); \
		if (__c->index!=0xffffffffffffffffull){ \
			sll_size_t __i=__c->index&0x7fffffffffffffffull; \
			__c->index=(sll_size_t)(*(__c->data+__i)); \
			*__idx=__i; \
		} \
		else{ \
			*__idx=__c->size; \
			__c->size++; \
			sll_allocator_resize((void**)(&(__c->data)),__c->size*sizeof(void*)); \
		} \
	} while (0)



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_CHECK
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_size_t index
 */
#define SLL_HANDLE_CONTAINER_CHECK(c,index) ((index)<(c)->size&&!(((sll_size_t)(*((c)->data+(index))))>>63))



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_CLEAR
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 */
#define SLL_HANDLE_CONTAINER_CLEAR(c) \
	do{ \
		sll_handle_container_t* __c=(c); \
		sll_allocator_release(__c->data); \
		__c->data=NULL; \
		__c->size=0; \
		__c->index=0xffffffffffffffffull; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_DEALLOC
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_size_t idx
 */
#define SLL_HANDLE_CONTAINER_DEALLOC(c,idx) \
	do{ \
		sll_handle_container_t* __c=(c); \
		sll_size_t __i=(idx); \
		*(__c->data+__i)=(void*)(__c->index|0x8000000000000000ull); \
		__c->index=__i; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_GET
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_size_t index
 * \ret void*
 */
#define SLL_HANDLE_CONTAINER_GET(c,index) ((index)<(c)->size&&!(((sll_size_t)(*((c)->data+(index))))>>63)?*((c)->data+(index)):NULL)



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_INIT
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 */
#define SLL_HANDLE_CONTAINER_INIT(c) \
	do{ \
		sll_handle_container_t* __c=(c); \
		__c->data=NULL; \
		__c->size=0; \
		__c->index=0xffffffffffffffffull; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_HANDLE_CONTAINER_INIT_STRUCT
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \type sll_handle_container_t
 */
#define SLL_HANDLE_CONTAINER_INIT_STRUCT {NULL,0,0xffffffffffffffffull}



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_ITER
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg __type__ type
 * \arg __identifier__ var
 * \arg __code__ code
 */
#define SLL_HANDLE_CONTAINER_ITER(c,type,var,code) \
	do{ \
		sll_handle_container_t* __c=(c); \
		for (sll_size_t __i=0;__i<__c->size;__i++){ \
			type* var=(type*)(*(__c->data+__i)); \
			if (((sll_size_t)var)>>63){ \
				continue; \
			} \
			code; \
		} \
	} while (0)



/**
 * \flags func macro
 * \name SLL_HANDLE_CONTAINER_ITER_CLEAR
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg __type__ type
 * \arg __identifier__ var
 * \arg __code__ code
 */
#define SLL_HANDLE_CONTAINER_ITER_CLEAR(c,type,var,code) \
	do{ \
		sll_handle_container_t* __c_tmp=(c); \
		SLL_HANDLE_CONTAINER_ITER(__c_tmp,type,var,code); \
		SLL_HANDLE_CONTAINER_CLEAR(__c_tmp); \
	} while (0)



/**
 * \flags macro var
 * \name SLL_MAP_CONTAINER_FLAG_XOR
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_MAP_CONTAINER_FLAG_XOR 0x8000000000000000ull



/**
 * \flags func macro
 * \name SLL_MAP_CONTAINER_HASH_XOR
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_size_t size
 * \ret void*
 */
#define SLL_MAP_CONTAINER_HASH_XOR(size) ((void*)(((sll_size_t)(size))|SLL_MAP_CONTAINER_FLAG_XOR))



/**
 * \flags func type
 * \name sll_container_callback_t
 * \group container
 * \desc Docs!
 * \arg void* elem
 */
typedef void (*sll_container_callback_t)(void* elem);



/**
 * \flags func type
 * \name sll_container_check_callback_t
 * \group container
 * \desc Docs!
 * \arg const void* elem
 * \ret sll_bool_t
 */
typedef sll_bool_t (*sll_container_check_callback_t)(const void* elem);



/**
 * \flags func type
 * \name sll_map_container_hash_callback_t
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg const void* key
 * \ret sll_size_t
 */
typedef sll_size_t (*sll_map_container_hash_callback_t)(const void* key);



/**
 * \flags func type
 * \name sll_map_container_equal_t
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg const void* value_a
 * \arg const void* value_b
 * \ret sll_bool_t
 */
typedef sll_bool_t (*sll_map_container_equal_t)(const void* value_a,const void* value_b);



/**
 * \flags func type
 * \name sll_map_container_filter_callback_t
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg const void* key
 * \arg void* value
 * \arg void* ctx
 * \ret sll_bool_t
 */
typedef sll_bool_t (*sll_map_container_filter_callback_t)(const void* key,void* value,void* ctx);



/**
 * \flags type
 * \name sll_container_t
 * \group container
 * \desc Docs!
 * \arg void* data
 * \arg sll_size_t size
 */
typedef struct _SLL_CONTAINER{
	void* data;
	sll_size_t size;
} sll_container_t;



/**
 * \flags type
 * \name sll_handle_container_t
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg void** data
 * \arg sll_size_t size
 * \arg sll_size_t index
 */
typedef struct _SLL_HANDLE_CONTAINER{
	void** data;
	sll_size_t size;
	sll_size_t index;
} sll_handle_container_t;



/**
 * \flags type
 * \name sll_map_container_entry_t
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg const void* key
 * \arg void* value
 * \arg sll_size_t _hash
 * \arg sll_bool_t _delete
 */
typedef struct _SLL_MAP_CONTAINER_ENTRY{
	const void* key;
	void* value;
	sll_size_t _hash;
	sll_bool_t _delete;
} sll_map_container_entry_t;



/**
 * \flags type union
 * \name sll_map_container_hash_data_t
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_hash_callback_t fn
 * \arg sll_size_t size
 */
typedef union _SLL_MAP_CONTAINER_HASH_DATA{
	sll_map_container_hash_callback_t fn;
	sll_size_t size;
} sll_map_container_hash_data_t;



/**
 * \flags type
 * \name sll_map_container_hash_t
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_hash_data_t data
 * \arg sll_bool_t is_function
 */
typedef struct _SLL_MAP_CONTAINER_HASH{
	sll_map_container_hash_data_t data;
	sll_bool_t is_function;
} sll_map_container_hash_t;



/**
 * \flags type
 * \name sll_map_container_t
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_entry_t* data
 * \arg sll_size_t* _indices
 * \arg sll_map_container_hash_t _hash
 * \arg sll_map_container_equal_t _eq_fn
 * \arg sll_size_t _elem_count
 * \arg sll_size_t _mask
 * \arg sll_size_t _rng
 */
typedef struct _SLL_MAP_CONTAINER{
	sll_map_container_entry_t* data;
	sll_size_t* _indices;
	sll_map_container_hash_t _hash;
	sll_map_container_equal_t _eq_fn;
	sll_size_t _elem_count;
	sll_size_t _mask;
	sll_size_t _rng;
} sll_map_container_t;



/**
 * \flags func
 * \name sll_container_clear
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 */
__SLL_EXTERNAL void sll_container_clear(sll_container_t* c);



/**
 * \flags func
 * \name sll_container_filter
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg sll_size_t elem_size
 * \arg sll_container_check_callback_t check
 * \arg sll_container_callback_t delete
 */
__SLL_EXTERNAL void sll_container_filter(sll_container_t* c,sll_size_t elem_size,sll_container_check_callback_t check,sll_container_callback_t delete);



/**
 * \flags func
 * \name sll_container_init
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 */
__SLL_EXTERNAL void sll_container_init(sll_container_t* c);



/**
 * \flags func
 * \name sll_container_iter
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg sll_size_t elem_size
 * \arg sll_container_callback_t callback
 */
__SLL_EXTERNAL void sll_container_iter(sll_container_t* c,sll_size_t elem_size,sll_container_callback_t callback);



/**
 * \flags func
 * \name sll_container_iter_clear
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg sll_size_t elem_size
 * \arg sll_container_callback_t callback
 */
__SLL_EXTERNAL void sll_container_iter_clear(sll_container_t* c,sll_size_t elem_size,sll_container_callback_t callback);



/**
 * \flags func
 * \name sll_container_push
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg const void* elem
 * \arg sll_size_t elem_size
 */
__SLL_EXTERNAL void sll_container_push(sll_container_t* c,const void* elem,sll_size_t elem_size);



/**
 * \flags check_output func
 * \name sll_handle_container_alloc
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_handle_container_alloc(sll_handle_container_t* c);



/**
 * \flags func
 * \name sll_handle_container_clear
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 */
__SLL_EXTERNAL void sll_handle_container_clear(sll_handle_container_t* c);



/**
 * \flags check_output func
 * \name sll_handle_container_check
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_size_t index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_handle_container_check(sll_handle_container_t* c,sll_size_t index);



/**
 * \flags func
 * \name sll_handle_container_dealloc
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_size_t index
 */
__SLL_EXTERNAL void sll_handle_container_dealloc(sll_handle_container_t* c,sll_size_t index);



/**
 * \flags check_output func
 * \name sll_handle_container_get
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_size_t index
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_handle_container_get(sll_handle_container_t* c,sll_size_t index);



/**
 * \flags func
 * \name sll_handle_container_init
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 */
__SLL_EXTERNAL void sll_handle_container_init(sll_handle_container_t* c);



/**
 * \flags func
 * \name sll_handle_container_iter
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_container_callback_t callback
 */
__SLL_EXTERNAL void sll_handle_container_iter(sll_handle_container_t* c,sll_container_callback_t callback);



/**
 * \flags func
 * \name sll_handle_container_iter_clear
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \arg sll_handle_container_t* c
 * \arg sll_container_callback_t callback
 */
__SLL_EXTERNAL void sll_handle_container_iter_clear(sll_handle_container_t* c,sll_container_callback_t callback);



/**
 * \flags func
 * \name sll_map_container_clear
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 */
__SLL_EXTERNAL void sll_map_container_clear(sll_map_container_t* c);



/**
 * \flags check_output func
 * \name sll_map_container_contains
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 * \arg const void* key
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_container_contains(sll_map_container_t* c,const void* key);



/**
 * \flags func
 * \name sll_map_container_deinit
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 */
__SLL_EXTERNAL void sll_map_container_deinit(sll_map_container_t* c);



/**
 * \flags func
 * \name sll_map_container_delete
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 * \arg const void* key
 * \arg void* default_
 * \ret void*
 */
__SLL_EXTERNAL void* sll_map_container_delete(sll_map_container_t* c,const void* key,void* default_);



/**
 * \flags func
 * \name sll_map_container_filter
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 * \arg sll_map_container_filter_callback_t callback
 * \arg void* ctx
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_map_container_filter(sll_map_container_t* c,sll_map_container_filter_callback_t callback,void* ctx);



/**
 * \flags check_output func
 * \name sll_map_container_get
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 * \arg const void* key
 * \arg void* default_
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_map_container_get(sll_map_container_t* c,const void* key,void* default_);



/**
 * \flags func
 * \name sll_map_container_init
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_hash_callback_t hash_fn
 * \arg sll_map_container_equal_t equal_fn
 * \arg sll_map_container_t* out
 */
__SLL_EXTERNAL void sll_map_container_init(sll_map_container_hash_callback_t hash_fn,sll_map_container_equal_t equal_fn,sll_map_container_t* out);



/**
 * \flags func
 * \name sll_map_container_iter
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 * \arg sll_container_callback_t callback
 */
__SLL_EXTERNAL void sll_map_container_iter(sll_map_container_t* c,sll_container_callback_t callback);



/**
 * \flags func
 * \name sll_map_container_iter_clear
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 * \arg sll_container_callback_t callback
 */
__SLL_EXTERNAL void sll_map_container_iter_clear(sll_map_container_t* c,sll_container_callback_t callback);



/**
 * \flags func
 * \name sll_map_container_set
 * \group container
 * \subgroup container-map
 * \desc Docs!
 * \arg sll_map_container_t* c
 * \arg const void* key
 * \arg void* value
 */
__SLL_EXTERNAL void sll_map_container_set(sll_map_container_t* c,const void* key,void* value);



#endif

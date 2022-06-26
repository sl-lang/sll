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
		for (sll_size_t __j=0;__j<__c->size;__j++){ \
			type var=(type)(*(__c->data+__j)); \
			if (check){ \
				delete; \
			} \
			else{ \
				*(__c->data+__i)=*(__c->data+__j); \
				__i++; \
			} \
		}; \
		if (__i!=__c->size){ \
			__c->size=__i; \
			sll_allocator_resize((void**)(&(__c->data)),__i*sizeof(type)); \
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
		for (sll_size_t __i=0;__i<__c->size;__i++){ \
			type var=(type)(*(__c->data+__i)); \
			code; \
		}; \
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
 * \arg __any__ elem
 */
#define SLL_CONTAINER_PUSH(c,elem) \
	do{ \
		sll_container_t* __c=(c); \
		__c->size++; \
		sll_allocator_resize((void**)(&(__c->data)),__c->size*sizeof(elem)); \
		*(__c->data+__c->size-1)=(void*)(elem); \
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
		}; \
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
 * \name SLL_HANDLE_CONTAINER_INIT_STRUCT
 * \group container
 * \subgroup container-handle
 * \desc Docs!
 * \type sll_handle_container_t
 */
#define SLL_HANDLE_CONTAINER_INIT_STRUCT {NULL,0,0xffffffffffffffffull}



/**
 * \flags type
 * \name sll_container_t
 * \group container
 * \desc Docs!
 * \arg void** data
 * \arg sll_size_t size
 */
typedef struct _SLL_CONTAINER{
	void** data;
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
 * \flags func
 * \name sll_container_clear
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 */
__SLL_EXTERNAL void sll_container_clear(sll_container_t* c);



#endif

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
		__c->size&=0xff; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_CONTAINER_DEINIT
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 */
#define SLL_CONTAINER_DEINIT(c) \
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
 * \arg __code__ code
 */
#define SLL_CONTAINER_FILTER(c,type,check,delete) \
	do{ \
		sll_container_t* __c=(c); \
		sll_size_t __sz=__c->size>>8; \
		sll_size_t __i=0; \
		for (sll_size_t __j=0;__j<__sz;__j++){ \
			type container_element=(type)(*(__c->data+__j)); \
			if (check){ \
				delete; \
			} \
			else{ \
				*(__c->data+__i)=*(__c->data+__j); \
				__i++; \
			} \
		}; \
		if (__i!=__sz){ \
			__c->size=(__i<<8)|(__c->size&0xff); \
			sll_allocator_resize((void**)(&(__c->data)),__i*(__c->size&0xff)); \
		} \
	} while (0)



/**
 * \flags func macro
 * \name SLL_CONTAINER_INIT
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg __type__ type
 */
#define SLL_CONTAINER_INIT(c,type) \
	do{ \
		sll_container_t* __c=(c); \
		__c->data=NULL; \
		__c->size=sizeof(type)&0xff; \
	} while (0)



/**
 * \flags func macro
 * \name SLL_CONTAINER_INIT_STRUCT
 * \group container
 * \desc Docs!
 * \arg __type__ type
 */
#define SLL_CONTAINER_INIT_STRUCT(type) {NULL,sizeof(type)&0xff}



/**
 * \flags func macro
 * \name SLL_CONTAINER_ITER
 * \group container
 * \desc Docs!
 * \arg sll_container_t* c
 * \arg __type__ type
 * \arg __code__ code
 */
#define SLL_CONTAINER_ITER(c,type,code) \
	do{ \
		sll_container_t* __c=(c); \
		sll_size_t __sz=__c->size>>8; \
		for (sll_size_t __i=0;__i<__sz;__i++){ \
			type container_element=(type)(*(__c->data+__i)); \
			code; \
		}; \
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
		sll_size_t __i=(__c->size>>8)+1; \
		sll_allocator_resize((void**)(&(__c->data)),__i*(__c->size&0xff)); \
		*(__c->data+__i-1)=(void*)(elem); \
		__c->size=(__i<<8)|(__c->size&0xff); \
	} while (0)



#endif

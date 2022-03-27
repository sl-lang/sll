#ifndef __SLL__INTERNAL_GC_H__
#define __SLL__INTERNAL_GC_H__ 1
#ifdef __SLL_BUILD_WINDOWS
#include <windows.h>
#endif
#include <immintrin.h>
#include <sll/_internal/scheduler.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
#define _ATOMIC_STORE(ptr,val) InterlockedExchange((LONG volatile*)(ptr),(val))
#define _ATOMIC_COMPARE_EXCHANGE(ptr,cmp,new) (InterlockedCompareExchange((LONG volatile*)(ptr),(new),*(cmp))==*(cmp))
#else
#define _ATOMIC_STORE(ptr,val) __atomic_store_n((ptr),(val),__ATOMIC_SEQ_CST)
#define _ATOMIC_COMPARE_EXCHANGE(ptr,cmp,new) __atomic_compare_exchange_n((ptr),(cmp),(new),0,__ATOMIC_SEQ_CST,__ATOMIC_RELAXED)
#endif



#define GC_PAGE_POOL_SIZE 32

#define GC_LOCK(o) \
	do{ \
		sll_object_t* __o=(o); \
		sll_object_type_t __b=__o->_f&0xffffff; \
		sll_object_type_t __v=__b|((_scheduler_internal_thread_index+1)<<24); \
		sll_object_type_t __tmp=__v; \
		if (!_ATOMIC_COMPARE_EXCHANGE((sll_object_type_t*)(&(__o->_f)),&__tmp,__v)){ \
			while (1){ \
				__tmp=__b; \
				if (_ATOMIC_COMPARE_EXCHANGE((sll_object_type_t*)(&(__o->_f)),&__tmp,__v)){ \
					break; \
				} \
				_mm_pause(); \
			} \
		} \
	} while (0)
#define GC_UNLOCK(o) \
	do{ \
		sll_object_t* __o=(o); \
		_ATOMIC_STORE((sll_object_type_t*)(&(__o->_f)),__o->_f&0xffffff); \
	} while (0)



typedef struct __GC_PAGE_HEADER{
	struct __GC_PAGE_HEADER* p;
	struct __GC_PAGE_HEADER* n;
	sll_size_t cnt;
} gc_page_header_t;



void _gc_release_data(void);



#endif

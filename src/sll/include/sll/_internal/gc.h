#ifndef __SLL_INTERNAL_GC_H__
#define __SLL_INTERNAL_GC_H__ 1
#ifdef __SLL_BUILD_WINDOWS
#include <windows.h>
#endif
#include <immintrin.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
#define _ATOMIC_STORE(ptr,val) InterlockedExchange((LONG volatile*)(ptr),val)
#define _ATOMIC_COMPARE_EXCHANGE(ptr,cmp,new) (InterlockedCompareExchange((LONG volatile*)(ptr),new,*(cmp))==*(cmp))
#else
#define _ATOMIC_STORE(ptr,val) __atomic_store_n((ptr),(val),__ATOMIC_SEQ_CST)
#define _ATOMIC_COMPARE_EXCHANGE(ptr,cmp,new) __atomic_compare_exchange_n((ptr),(cmp),(new),0,__ATOMIC_SEQ_CST,__ATOMIC_RELAXED)
#endif



#define GC_GET_NEXT_OBJECT(o) ((sll_object_t*)((o)->dt.s.v))
#define GC_SET_NEXT_OBJECT(o,n) ((o)->dt.s.v=SLL_CHAR((n)))

#define GC_OBJECT_POOL_ALLOC_SIZE 65536

#define GC_FLAG_LOCK SLL_OBJECT_FLAG_RESERVED0

#define GC_LOCK(o) \
	do{ \
		sll_object_t* __o=(o); \
		sll_object_type_t __t=SLL_OBJECT_GET_TYPE(__o); \
		sll_object_type_t __v=__t|GC_FLAG_LOCK; \
		while (1){ \
			sll_object_type_t __tmp=__t; \
			if (_ATOMIC_COMPARE_EXCHANGE((sll_object_type_t*)(&(__o->t)),&__tmp,__v)){ \
				break; \
			} \
			_mm_pause(); \
		} \
	} while (0)
#define GC_UNLOCK(o) \
	do{ \
		sll_object_t* __o=(o); \
		_ATOMIC_STORE((sll_object_type_t*)(&(__o->t)),SLL_OBJECT_GET_TYPE(__o)); \
	} while (0)



void _gc_release_data(void);



#endif

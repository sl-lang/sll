#ifndef __SLL__INTERNAL_GC_H__
#define __SLL__INTERNAL_GC_H__ 1
#include <sll/_internal/common.h>
#include <sll/_size_types.h>
#include <sll/api/time.h>
#include <sll/gc.h>
#include <sll/types.h>



#define GC_PAGE_POOL_SIZE 4
#define GC_PAGE_INIT_POOL_SIZE 4
#define GC_PAGE_DYNAMIC_POOL_SIZE 2

#define GC_FLAG_HAS_WEAKREF 1
#define GC_FLAG_IN_FAST_POOL 2
#define GC_FLAG_STATIC 4
#define GC_FLAG_IN_FAST_ROOT_POOL 8

#define GC_MEMORY_PAGE_SIZE_SHIFT 15
#define GC_MEMORY_PAGE_SIZE (1ull<<GC_MEMORY_PAGE_SIZE_SHIFT)
#define GC_MEMORY_PAGE_HEADER(o) ((gc_page_header_t*)PTR(ADDR(o)&(0-GC_MEMORY_PAGE_SIZE)))

#define GC_OBJECT_POOL_SIZE 2048

#define GC_FAST_OBJECT_POOL_SIZE 64

#define GC_PAGE_HEADER_DEALLOCATION_THRESHOLD 256

#define GC_PAGE_HEADER_CAN_DELETE(pg) ((pg)->cnt==1)
#define GC_PAGE_HEADER_INCREASE(pg) \
	do{ \
		gc_page_header_t* __pg=(pg); \
		__pg->cnt+=2; \
		if (__pg->cnt>=(GC_PAGE_HEADER_DEALLOCATION_THRESHOLD<<1)){ \
			__pg->cnt|=1; \
		} \
	} while (0)
#define GC_PAGE_HEADER_DECREASE(pg) \
	do{ \
		(pg)->cnt-=2; \
	} while (0)

#define GC_GET_PREV_OBJECT(o) ((sll_object_t)PTR(((o)->_data&0x1fffffffffffull)<<3))
#define GC_GET_NEXT_OBJECT(o) ((sll_object_t)PTR((((o)->_flags>>6)|(((o)->_data>>45)<<26))<<3))
#define GC_SET_PREV_OBJECT(o,prev) \
	do{ \
		SLL_ASSERT((ADDR(prev)&0xfffffffffff8ull)==ADDR(prev)); \
		(o)->_data=((o)->_data&0xffffe00000000000ull)|(ADDR(prev)>>3); \
	} while (0)
#define GC_SET_NEXT_OBJECT(o,next) \
	do{ \
		SLL_ASSERT((ADDR(next)&0xfffffffffff8ull)==ADDR(next)); \
		(o)->_flags=((o)->_flags&0x3f)|((__SLL_U32)(ADDR(next)<<3)); \
		(o)->_data=((o)->_data&0x1fffffffffffull)|((ADDR(next)<<16)&0xffffe00000000000ull); \
	} while (0)
#define GC_CLEAR_OBJECTS(o) \
	do{ \
		(o)->_flags&=0x3f; \
		(o)->_data=0; \
	} while (0)

#define GC_GARBAGE_COLLECTION_INTERVAL 1024

#define GC_ROOTS_POINTER_SHIFT 45
#define GC_ROOTS_LENGTH_SHIFT (64-GC_ROOTS_POINTER_SHIFT)
#define GC_ENCODE_ROOT(data,length) ((length)|((ADDR(data)>>3)<<GC_ROOTS_LENGTH_SHIFT))
#define GC_GET_ROOT(data) ((const sll_object_t*)PTR(((data)>>GC_ROOTS_LENGTH_SHIFT)<<3))
#define GC_GET_LENGTH(data) ((sll_size_t)((data)&((1<<GC_ROOTS_LENGTH_SHIFT)-1)))

#define GC_IS_MARKED(o) (((o)->_flags&GC_FLAG_STATIC)||(((o)->_flags>>5)&1)==_gc_data.object_marker_signature)
#define GC_SET_MARKED(o) \
	do{ \
		(o)->_flags=((o)->_flags&0xffffffdf)|(_gc_data.object_marker_signature<<5); \
	} while (0)

#define GC_RELEASE_CHECK_ZERO_REF(o) \
	do{ \
		sll_object_t __o=(o); \
		if (!SLL_GET_OBJECT_REFERENCE_COUNTER(__o)){ \
			if (!_gc_data.cleanup_in_progress){ \
				sll__gc_error(__o); \
			} \
		} \
		else{ \
			__o->reference_count--; \
			if (!SLL_GET_OBJECT_REFERENCE_COUNTER(__o)){ \
				sll__release_object_internal(__o); \
			} \
		} \
	} while (0)

#define GC_FAST_ROOT_DATA_COUNT 512
#define GC_FAST_ROOT_IS_OBJECT(x) (x&&!(ADDR(x)&1))
#define GC_FAST_ROOT_GET_NEXT_INDEX(x) ((fast_root_index_t)(ADDR(x)>>1))
#define GC_FAST_ROOT_SET_NEXT_INDEX(i) PTR(((i)<<1)|1)

#define GC_ROOT_COUNTER_SHIFT 48

#define GC_IS_ANY_ROOT(x) (!!((x)->reference_count>>GC_ROOT_COUNTER_SHIFT))
#define GC_DECREASE_ROOT(x) ((x)->reference_count-=1ull<<GC_ROOT_COUNTER_SHIFT)
#define GC_INCREASE_ROOT(x) ((x)->reference_count+=1ull<<GC_ROOT_COUNTER_SHIFT)

#define GC_PAGE_OBJECT_COUNT ((GC_MEMORY_PAGE_SIZE-sizeof(gc_page_header_t))/sizeof(struct _SLL_OBJECT))
#define GC_ITER_PAGE_OBJECTS(page) for (sll_object_t current=PTR(ADDR((page))+sizeof(gc_page_header_t)),end=current+GC_PAGE_OBJECT_COUNT;current<end;current++)



typedef __SLL_U16 fast_root_index_t;



typedef __SLL_U8 gc_fast_object_pool_length_t;



typedef __SLL_U32 gc_memory_page_data_length_t;



typedef __SLL_U64 gc_multiple_root_t;



typedef struct _GC_PAGE_HEADER{
	sll_size_t cnt;
	sll_size_t garbage_cnt;
	struct _GC_PAGE_HEADER* prev;
	struct _GC_PAGE_HEADER* next;
} gc_page_header_t;



typedef struct _GC_MEMORY_PAGE_DATA{
	gc_page_header_t* root;
	gc_page_header_t* data[GC_PAGE_POOL_SIZE];
	gc_memory_page_data_length_t length;
} gc_memory_page_data_t;



typedef struct _GC_OBJECT_POOL{
	sll_object_t data[GC_OBJECT_POOL_SIZE];
	sll_object_pool_index_t length;
} gc_object_pool_t;



typedef struct _GC_FAST_OBJECT_POOL{
	sll_object_t data[GC_FAST_OBJECT_POOL_SIZE];
	gc_fast_object_pool_length_t read;
	gc_fast_object_pool_length_t write;
	gc_fast_object_pool_length_t space;
} gc_fast_object_pool_t;



typedef struct _GC_ROOT_DATA{
	sll_object_t single;
	gc_multiple_root_t* multiple;
	sll_size_t multiple_length;
	sll_object_t fast[GC_FAST_ROOT_DATA_COUNT];
	fast_root_index_t fast_count;
	fast_root_index_t fast_empty_index;
} gc_root_data_t;



typedef struct _GC_DATA{
	sll_time_t time;
	sll_bool_t object_marker_signature;
	sll_bool_t enabled;
	sll_bool_t cleanup_in_progress;
} gc_data_t;



void _gc_release_data(void);



#endif

#ifndef __SLL__INTERNAL_GC_H__
#define __SLL__INTERNAL_GC_H__ 1
#include <sll/_internal/common.h>
#include <sll/_size_types.h>
#include <sll/api/time.h>
#include <sll/types.h>



#define GC_PAGE_POOL_SIZE 4
#define GC_PAGE_INIT_POOL_SIZE 4
#define GC_PAGE_DYNAMIC_POOL_SIZE 2

#define GC_FLAG_HAS_WEAKREF 1
#define GC_FLAG_IN_FAST_POOL 2
#define GC_FLAG_STATIC 4

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

#define GC_GET_PREV_OBJECT(o) ((sll_object_t*)PTR(((o)->_data&0x1fffffffffffull)<<3))
#define GC_GET_NEXT_OBJECT(o) ((sll_object_t*)PTR((((o)->_flags>>6)|(((o)->_data>>45)<<26))<<3))
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

#define GC_GARBAGE_COLLECTION_INTERVAL 256

#define GC_ROOTS_POINTER_SHIFT 45
#define GC_ROOTS_LENGTH_SHIFT (64-GC_ROOTS_POINTER_SHIFT)
#define GC_ENCODE_ROOT(data,length) ((length)|((ADDR(data)>>3)<<GC_ROOTS_LENGTH_SHIFT))
#define GC_GET_ROOT(data) ((sll_object_t*const*)PTR(((data)>>GC_ROOTS_LENGTH_SHIFT)<<3))
#define GC_GET_LENGTH(data) ((sll_size_t)((data)&((1<<GC_ROOTS_LENGTH_SHIFT)-1)))

#define GC_IS_MARKED(o) ((((o)->_flags>>5)&1)==_gc_garbage_collector.signature)
#define GC_SET_MARKED(o) \
	do{ \
		(o)->_flags=((o)->_flags&0xffffffdf)|(_gc_garbage_collector.signature<<5); \
	} while (0)



typedef struct _GC_PAGE_HEADER{
	sll_size_t cnt;
	sll_size_t garbage_cnt;
	struct _GC_PAGE_HEADER* prev;
	struct _GC_PAGE_HEADER* next;
} gc_page_header_t;



typedef struct _GC_OBJECT_POOL{
	sll_object_t* data[GC_OBJECT_POOL_SIZE];
	sll_array_length_t length;
} gc_object_pool_t;



typedef struct _GC_FAST_OBJECT_POOL{
	sll_object_t* data[GC_FAST_OBJECT_POOL_SIZE];
	__SLL_U8 read;
	__SLL_U8 write;
	__SLL_U8 space;
} gc_fast_object_pool_t;



typedef struct _GC_GARBAGE_COLLECTOR_DATA{
	sll_object_t* single;
	__SLL_U64* multiple;
	sll_size_t multiple_length;
	sll_time_t time;
	sll_bool_t signature;
	sll_bool_t enabled;
} gc_garbage_collector_data_t;



void _gc_release_data(void);



#endif

#ifndef __SLL__INTERNAL_STRING_H__
#define __SLL__INTERNAL_STRING_H__ 1
#include <sll/_internal/attribute.h>
#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/allocator.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>



#define STRING_DATA_PTR(x) ASSUME_ALIGNED(x,4,0)

#define INIT_PADDING(v,l) (*((wide_data_t*)((v)+((l)&0xfffffffffffffff8ull)))=0)



static __SLL_FORCE_INLINE sll_bool_t STRING_EQUAL(const sll_string_t* a,const sll_string_t* b){
	if (a->length!=b->length||a->checksum!=b->checksum){
		return 0;
	}
	if (!a->length){
		return 1;
	}
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	sll_string_length_t l=(a->length+7)>>3;
#ifdef __SLL_BUILD_DARWIN
	while (l){
		if (*ap!=*bp){
			return 0;
		}
		l--;
		ap++;
		bp++;
	}
	return 1;
#else
	while (l>3){
		if (_mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_lddqu_si256((const __m256i*)ap),_mm256_lddqu_si256((const __m256i*)bp)))!=0xffffffff){
			return 0;
		}
		l-=4;
		ap+=4;
		bp+=4;
	}
	if (l>1){
		if (_mm_movemask_epi8(_mm_cmpeq_epi8(_mm_lddqu_si128((const __m128i*)ap),_mm_lddqu_si128((const __m128i*)bp)))!=0xffff){
			return 0;
		}
		l-=2;
		ap+=2;
		bp+=2;
	}
	return (l?*ap==*bp:1);
#endif
}



static __SLL_FORCE_INLINE void STRING_INIT_STACK(sll_string_t* o){
	o->length=0;
	o->checksum=0;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(0)*sizeof(sll_char_t));
	INIT_PADDING(o->data,0);
	sll_allocator_move((void**)(&(o->data)),SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
}



#endif

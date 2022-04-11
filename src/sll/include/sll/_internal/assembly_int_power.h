#ifndef __SLL__INTERNAL_ASSEMBLY_INT_POWER_H__
#define __SLL__INTERNAL_ASSEMBLY_INT_POWER_H__ 1
#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_size_types.h>
#include <sll/assembly.h>
#include <sll/types.h>



static __SLL_FORCE_INLINE sll_integer_t _assembly_decode_compressed_int(sll_compressed_integer_t v){
	sll_integer_t off=v>>8;
	if (v&128){
		off=-off;
	}
	sll_integer_t o=(1ull<<(v&63))+off;
	if (v&64){
		o=-o;
	}
	return o;
}



static __SLL_FORCE_INLINE void _assembly_optimize_int_power(sll_assembly_instruction_t* ai){
	SLL_ASSERT(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT);
	sll_integer_t v=ai->dt.i;
	__SLL_U32 sgn=0;
	if (v<0){
		v=-v;
		sgn=1;
	}
	__SLL_S32 off=0;
	__SLL_U32 s=FIND_LAST_SET_BIT((__SLL_U64)v);
	if (v&(v-1)){
		sll_size_t pw=1ull<<s;
		__SLL_S32 n_pw=(__SLL_S32)((pw<<1)-v);
		off=(__SLL_S32)(v-pw);
		if (n_pw<off){
			off=n_pw;
			sgn|=2;
			s++;
		}
		if (off>>24){
			return;
		}
	}
	sll_compressed_integer_t ci=s|(sgn<<6)|(off<<8);
	if (ci>v){
		return;
	}
	ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED;
	ai->dt.ci=ci;
}



#endif

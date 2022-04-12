#ifndef __SLL__INTERNAL_ASSEMBLY_COMPRESS_INTEGER_H__
#define __SLL__INTERNAL_ASSEMBLY_COMPRESS_INTEGER_H__ 1
#include <sll/_internal/common.h>
#include <sll/compressed_integer.h>
#include <sll/assembly.h>
#include <sll/types.h>



static __SLL_FORCE_INLINE void _assembly_optimize_int(sll_assembly_instruction_t* ai){
	SLL_ASSERT(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT);
	sll_integer_t ci=sll_compress_integer(ai->dt.i);
	if (ci!=ai->dt.i){
		ai->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED;
		ai->dt.ci=(sll_compressed_integer_t)ci;
	}
}



#endif

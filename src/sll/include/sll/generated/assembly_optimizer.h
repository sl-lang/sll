#ifndef __SLL_GENERATED_ASSEMBLY_OPTIMIZER_H__
#define __SLL_GENERATED_ASSEMBLY_OPTIMIZER_H__ 1
#include <sll/_internal/assembly.h>
#include <sll/assembly.h>
#include <sll/types.h>



static __SLL_FORCE_INLINE void _optimize_assembly(sll_assembly_instruction_t** st,sll_assembly_instruction_t* nop){
	if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])!=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
			if (st[1]->dt.i==-1){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE;
			}
			else if (st[1]->dt.i==0){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO;
			}
			else if (st[1]->dt.i==1){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE;
			}
			else if (st[1]->dt.i==2){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO;
			}
			else if (st[1]->dt.i==3){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE;
			}
			else if (st[1]->dt.i==4){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR;
			}
		}
	}
	if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_POP){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL){
			goto _shift_2;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if (st[0]->dt.v==st[1]->dt.v){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				goto _shift_1;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if (st[0]->dt.v==st[1]->dt.v){
				goto _shift_2;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE;
			st[1]->dt.v=st[0]->dt.v;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO;
			st[1]->dt.v=st[0]->dt.v;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE;
			st[1]->dt.v=st[0]->dt.v;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO;
			st[1]->dt.v=st[0]->dt.v;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE;
			st[1]->dt.v=st[0]->dt.v;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR;
			st[1]->dt.v=st[0]->dt.v;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_INV||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY){
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[2])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
				if (st[0]->dt.v==st[2]->dt.v){
					st[2]->t=SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])|SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE;
					goto _shift_2;
				}
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
			if (st[0]->dt.ac==0){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO;
				goto _shift_1;
			}
			else if (st[0]->dt.ac==1){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE;
				goto _shift_1;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_INC;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_INC;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO){
			if (st[0]->dt.ac==0){
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL;
				goto _shift_1;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
			st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT){
		if (st[0]->dt.i==0){
			st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(st[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if (st[0]->dt.v==st[1]->dt.v){
				st[0]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
				st[1]->t=SLL_ASSEMBLY_INSTRUCTION_TYPE_POP;
			}
		}
	}
	return;
_shift_1:
	st[0]->t=ASSEMBLY_INSTRUCTION_TYPE_NOP;
	st[0]=st[1];
	st[1]=st[2];
	st[2]=st[3];
	st[3]=st[4];
	return;
_shift_2:
	st[0]->t=ASSEMBLY_INSTRUCTION_TYPE_NOP;
	st[1]->t=ASSEMBLY_INSTRUCTION_TYPE_NOP;
	st[0]=st[2];
	st[1]=st[3];
	st[2]=st[4];
	st[3]=nop;
}



#endif

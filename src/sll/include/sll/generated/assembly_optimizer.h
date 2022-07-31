#ifndef __SLL_GENERATED_ASSEMBLY_OPTIMIZER_H__
#define __SLL_GENERATED_ASSEMBLY_OPTIMIZER_H__ 1
#include <sll/_internal/assembly.h>
#include <sll/assembly.h>
#include <sll/types.h>



static __SLL_FORCE_INLINE void _optimize_assembly(sll_assembly_data_t* assembly_data,sll_assembly_instruction_t* stack,sll_assembly_instruction_t nop){
	if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])!=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])!=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])!=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
			if (stack[1]->data.int_==-1){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE;
			}
			else if (stack[1]->data.int_==0){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO;
			}
			else if (stack[1]->data.int_==1){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE;
			}
			else if (stack[1]->data.int_==2){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO;
			}
			else if (stack[1]->data.int_==3){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE;
			}
			else if (stack[1]->data.int_==4){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR;
			}
			else{
				_assembly_optimize_int(stack[1]);
			}
		}
	}
	if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_POP){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL){
			goto _shift_2;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if (stack[0]->data.variable==stack[1]->data.variable){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE;
				goto _shift_1;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if (stack[0]->data.variable==stack[1]->data.variable){
				goto _shift_2;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE;
			stack[1]->data.variable=stack[0]->data.variable;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO;
			stack[1]->data.variable=stack[0]->data.variable;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE;
			stack[1]->data.variable=stack[0]->data.variable;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO;
			stack[1]->data.variable=stack[0]->data.variable;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE;
			stack[1]->data.variable=stack[0]->data.variable;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR;
			stack[1]->data.variable=stack[0]->data.variable;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_INV||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY||SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY){
			if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[2])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
				if (stack[0]->data.variable==stack[2]->data.variable){
					stack[2]->type=SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])|SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE;
					goto _shift_2;
				}
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
			if (stack[0]->data.arg_count==0){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO;
				goto _shift_1;
			}
			else if (stack[0]->data.arg_count==1){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE;
				goto _shift_1;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_INC;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_INC;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO){
			if (stack[0]->data.arg_count==0){
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL;
				goto _shift_1;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR;
			goto _shift_1;
		}
		else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD){
			stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR;
			goto _shift_1;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT){
		if (stack[0]->data.int_==0){
			stack[0]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO;
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[1])==SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP){
			if (stack[0]->data.variable==stack[1]->data.variable){
				stack[0]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL;
				stack[1]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_POP;
			}
		}
	}
	else if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(stack[0])==SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS){
		if (!assembly_data->string_table.data[stack[0]->data.string_index].length){
			stack[0]->type=SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_EMPTY_STRING;
		}
	}
	return;
_shift_1:
	stack[0]->type=ASSEMBLY_INSTRUCTION_TYPE_NOP;
	stack[0]=stack[1];
	stack[1]=stack[2];
	stack[2]=stack[3];
	stack[3]=stack[4];
	return;
_shift_2:
	stack[0]->type=ASSEMBLY_INSTRUCTION_TYPE_NOP;
	stack[1]->type=ASSEMBLY_INSTRUCTION_TYPE_NOP;
	stack[0]=stack[2];
	stack[1]=stack[3];
	stack[2]=stack[4];
	stack[3]=nop;
}



#endif

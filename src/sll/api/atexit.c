#include <sll/_internal/api.h>
#include <sll/_internal/atexit.h>
#include <sll/_internal/common.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/types.h>
#include <sll/vm.h>



static atexit_function_t** _atexit_data=NULL;
static sll_array_length_t _atexit_data_len=0;



static void _delete_atexit_function(atexit_function_t* af){
	while (af->all){
		af->all--;
		sll_release_object(af->al[af->all]);
	}
	sll_deallocate(af);
}



void _atexit_execute(void){
	while (_atexit_data_len){
		_atexit_data_len--;
		atexit_function_t* af=*(_atexit_data+_atexit_data_len);
		sll_release_object(sll_execute_function(af->fn,af->al,af->all,0));
		_delete_atexit_function(af);
	}
	sll_deallocate(_atexit_data);
	_atexit_data=NULL;
}



__API_FUNC(atexit_register){
	if (!a){
		return;
	}
	_atexit_data_len++;
	_atexit_data=sll_reallocate(_atexit_data,_atexit_data_len*sizeof(atexit_function_t*));
	atexit_function_t* af=sll_allocate(sizeof(atexit_function_t)+bc*sizeof(sll_object_t*));
	af->fn=a;
	af->all=bc;
	for (sll_arg_count_t i=0;i<bc;i++){
		SLL_ACQUIRE(*(b+i));
		af->al[i]=*(b+i);
	}
	*(_atexit_data+_atexit_data_len-1)=af;
}



__API_FUNC(atexit_unregister){
	if (!a){
		return 0;
	}
	sll_bool_t o=0;
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<_atexit_data_len;j++){
		atexit_function_t* af=*(_atexit_data+j);
		if (af->fn==a){
			o=1;
			_delete_atexit_function(af);
		}
		else{
			*(_atexit_data+i)=af;
			i++;
		}
	}
	_atexit_data_len=i;
	_atexit_data=sll_reallocate(_atexit_data,_atexit_data_len*sizeof(atexit_function_t*));
	return o;
}

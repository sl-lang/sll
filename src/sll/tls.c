#include <sll/_internal/common.h>
#include <sll/_internal/scheduler.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/static_object.h>
#include <sll/tls.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_free_tls(sll_tls_object_t* tls){
	while (tls->length){
		tls->length--;
		SLL_RELEASE((tls->data+tls->length)->value);
	}
	sll_deallocate(tls->data);
	tls->data=NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t** sll_tls_get(sll_tls_object_t* tls){
	if (!tls->length){
		tls->length=1;
		tls->data=sll_allocate(sizeof(sll_tls_value_t));
		tls->data->thread_index=_scheduler_current_thread_index;
		tls->data->value=SLL_ACQUIRE_STATIC_INT(0);
		return &(tls->data->value);
	}
_retry:;
	sll_tls_object_length_t i=_scheduler_current_thread_index%tls->length;
	sll_tls_object_length_t c=tls->length;
	while (c){
		sll_tls_value_t* v=tls->data+i;
		if (!v->value){
			v->thread_index=_scheduler_current_thread_index;
			v->value=SLL_ACQUIRE_STATIC_INT(0);
			return &(v->value);
		}
		if (v->thread_index==_scheduler_current_thread_index){
			return &(v->value);
		}
		i++;
		if (i==tls->length){
			i=0;
		}
		c--;
	}
	tls->length++;
	sll_tls_value_t* n_dt=sll_zero_allocate(tls->length*sizeof(sll_tls_value_t));
	for (i=0;i<tls->length-1;i++){
		sll_tls_value_t* v=tls->data+i;
		if (v->value){
			*(n_dt+(v->thread_index%tls->length))=*v;
		}
	}
	sll_deallocate(tls->data);
	tls->data=n_dt;
	goto _retry;
}

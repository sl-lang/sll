#include <sll/_internal/common.h>
#include <sll/_internal/scheduler.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_free_tls(sll_tls_object_t* tls){
	while (tls->sz){
		tls->sz--;
		sll_release_object((tls->dt+tls->sz)->v);
	}
	sll_deallocate(tls->dt);
	tls->dt=NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t** sll_tls_get(sll_tls_object_t* tls){
	if (!tls->sz){
		tls->sz=1;
		tls->dt=sll_allocate(sizeof(sll_tls_value_t));
		tls->dt->t=_scheduler_current_thread_index;
		tls->dt->v=SLL_ACQUIRE_STATIC_INT(0);
		return &(tls->dt->v);
	}
_retry:;
	sll_array_length_t i=_scheduler_current_thread_index%tls->sz;
	sll_array_length_t c=tls->sz;
	while (c){
		sll_tls_value_t* v=tls->dt+i;
		if (!v->v){
			v->t=_scheduler_current_thread_index;
			v->v=SLL_ACQUIRE_STATIC_INT(0);
			return &(v->v);
		}
		if (v->t==_scheduler_current_thread_index){
			return &(v->v);
		}
		i++;
		if (i==tls->sz){
			i=0;
		}
		c--;
	}
	tls->sz++;
	sll_tls_value_t* n_dt=sll_zero_allocate(tls->sz*sizeof(sll_tls_value_t));
	for (i=0;i<tls->sz-1;i++){
		sll_tls_value_t* v=tls->dt+i;
		if (v->v){
			*(n_dt+(v->t%tls->sz))=*v;
		}
	}
	sll_deallocate(tls->dt);
	tls->dt=n_dt;
	goto _retry;
}

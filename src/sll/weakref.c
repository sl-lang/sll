#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/weakref.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/types.h>
#include <sll/weakref.h>



static weakref_key_pair_t** _weakref_data=NULL;
static sll_array_length_t _weakref_data_len=0;
static sll_bool_t _weakref_cb=0;
static sll_weak_reference_t _weakref_next=NULL;



static sll_weak_reference_t _create_new(addr_t obj){
	_weakref_data_len++;
	_weakref_data=sll_reallocate(_weakref_data,_weakref_data_len*sizeof(weakref_key_pair_t*));
	_weakref_next=PTR(ADDR(_weakref_next)+1);
	weakref_key_pair_t* kp=sll_allocate(sizeof(weakref_key_pair_t));
	kp->obj=obj;
	kp->wr=_weakref_next;
	kp->cb=NULL;
	kp->arg=NULL;
	*(_weakref_data+_weakref_data_len-1)=kp;
	return _weakref_next;
}



void _weakref_cleanup_data(void){
	while (_weakref_data_len){
		_weakref_data_len--;
		sll_deallocate(*(_weakref_data+_weakref_data_len));
	}
	sll_deallocate(_weakref_data);
	_weakref_data=NULL;
	_weakref_cb=0;
	_weakref_next=NULL;
}



void _weakref_delete(sll_object_t* o){
	SLL_ASSERT(o->_flags&GC_FLAG_HAS_WEAKREF);
	sll_array_length_t i=0;
	weakref_key_pair_t** kp=_weakref_data;
	for (sll_array_length_t j=0;j<_weakref_data_len;j++){
		weakref_key_pair_t* v=*kp;
		if (v->obj==ADDR(o)){
			if (v->cb){
				v->cb(v->wr,o,v->arg);
			}
			sll_deallocate(v);
		}
		else{
			*(_weakref_data+i)=v;
			i++;
		}
		kp++;
	}
	_weakref_data_len=i;
	_weakref_data=sll_reallocate(_weakref_data,_weakref_data_len*sizeof(weakref_key_pair_t*));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_weakref_clone(sll_weak_reference_t wr){
	weakref_key_pair_t** kp=_weakref_data;
	for (sll_array_length_t i=0;i<_weakref_data_len;i++){
		weakref_key_pair_t* k=*kp;
		if (k->wr==wr){
			return _create_new(k->obj);
		}
		kp++;
	}
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_weakref_create(sll_object_t* o){
	o->_flags|=GC_FLAG_HAS_WEAKREF;
	return _create_new(ADDR(o));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_weakref_delete(sll_weak_reference_t wr){
	sll_array_length_t i=0;
	weakref_key_pair_t** kp=_weakref_data;
	while (i<_weakref_data_len){
		if ((*kp)->wr==wr){
			sll_deallocate(*kp);
			i++;
			_weakref_data_len--;
			while (i<_weakref_data_len){
				*(_weakref_data+i)=*(_weakref_data+i+1);
			}
			_weakref_data=sll_reallocate(_weakref_data,_weakref_data_len*sizeof(weakref_key_pair_t*));
			return 1;
		}
		i++;
		kp++;
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_weakref_get(sll_weak_reference_t wr){
	weakref_key_pair_t** kp=_weakref_data;
	for (sll_array_length_t i=0;i<_weakref_data_len;i++){
		if ((*kp)->wr==wr){
			return PTR((*kp)->obj);
		}
		kp++;
	}
	return NULL;
}



__SLL_EXTERNAL void sll_weakref_set_callback(sll_weak_reference_t wr,sll_weak_ref_destructor_t cb,void* arg){
	weakref_key_pair_t** kp=_weakref_data;
	for (sll_array_length_t i=0;i<_weakref_data_len;i++){
		weakref_key_pair_t* v=*kp;
		if (v->wr==wr){
			v->cb=cb;
			v->arg=arg;
			return;
		}
		kp++;
	}
}

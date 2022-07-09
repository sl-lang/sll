#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/memory.h>
#include <sll/platform/util.h>
#include <sll/types.h>



static sll_size_t _hash_key(const sll_map_container_t* c,const void* key){
	sll_size_t out=0;
	if (c->_hash.is_function){
		out=c->_hash.data.fn(key);
	}
	else if (c->_hash.data.size){
		SLL_UNIMPLEMENTED();
	}
	else{
		out=ADDR(key);
	}
	return ROTATE_BITS_RIGHT64(out*c->_rng,c->_rng&63);
}



static sll_size_t _lookup_entry(const sll_map_container_t* c,sll_size_t hash,const void* key){
	sll_size_t i=hash&c->_mask;
	for (sll_size_t j=0;j<c->_elem_count;j++){
		sll_size_t k=*(c->_indices+i);
		SLL_ASSERT(k==0xffffffffffffffffull||k<c->_elem_count);
		if (k!=0xffffffffffffffffull&&!(c->data+k)->_delete&&(c->data+k)->_hash==hash){
			const void* entry_key=(c->data+k)->key;
			if ((c->_hash.is_function&&c->_eq_fn(entry_key,key))||(!c->_hash.is_function&&c->_hash.data.size&&sll_compare_data(entry_key,key,c->_hash.data.size))||(!c->_hash.is_function&&!c->_hash.data.size&&entry_key==key)){
				return i;
			}
		}
		i=(i+1)&c->_mask;
	}
	return 0xffffffffffffffffull;
}



static void _index_element(const sll_map_container_t* c,sll_size_t i){
	sll_size_t j=(c->data+i)->_hash&c->_mask;
	while (c->_indices[j]!=0xffffffffffffffffull){
		j=(j+1)&c->_mask;
	}
	c->_indices[j]=i;
}



__SLL_EXTERNAL void sll_map_container_clear(sll_map_container_t* c){
	sll_allocator_release(c->data);
	sll_deallocate(c->_indices);
	c->data=NULL;
	c->_indices=NULL;
	c->_elem_count=0;
	c->_mask=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_container_contains(sll_map_container_t* c,const void* key){
	return (_lookup_entry(c,_hash_key(c,key),key)!=0xffffffffffffffffull);
}



__SLL_EXTERNAL void sll_map_container_deinit(sll_map_container_t* c){
	sll_allocator_release(c->data);
	sll_deallocate(c->_indices);
	sll_zero_memory(c,sizeof(sll_map_container_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_map_container_delete(sll_map_container_t* c,const void* key,void* default_){
	sll_size_t index=_lookup_entry(c,_hash_key(c,key),key);
	if (index==0xffffffffffffffffull){
		return default_;
	}
	sll_size_t data_index=*(c->_indices+index);
	SLL_ASSERT(data_index<c->_elem_count);
	SLL_ASSERT(index<=c->_mask);
	*(c->_indices+index)=0xffffffffffffffffull;
	(c->data+data_index)->_delete=1;
	return (c->data+data_index)->value;
}



__SLL_EXTERNAL sll_bool_t sll_map_container_filter(sll_map_container_t* c,sll_map_container_filter_callback_t callback,void* ctx){
	sll_bool_t out=0;
	for (sll_size_t i=0;i<c->_elem_count;i++){
		sll_map_container_entry_t* e=c->data+i;
		if (e->_delete||!callback(e->key,e->value,ctx)){
			continue;
		}
		out=1;
		e->_delete=1;
		sll_size_t j=0;
		while (*(c->_indices+j)!=i){
			j++;
		}
		SLL_ASSERT(j<=c->_mask);
		*(c->_indices+j)=0;
	}
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_map_container_get(sll_map_container_t* c,const void* key,void* default_){
	sll_size_t index=_lookup_entry(c,_hash_key(c,key),key);
	return (index!=0xffffffffffffffffull?(c->data+(*(c->_indices+index)))->value:default_);
}



__SLL_EXTERNAL void sll_map_container_init(sll_map_container_hash_callback_t hash_fn,sll_map_container_equal_t equal_fn,sll_map_container_t* out){
	out->data=NULL;
	out->_indices=NULL;
	out->_hash.is_function=0;
	if (!hash_fn){
		out->_hash.data.size=0;
	}
	else if (ADDR(hash_fn)&SLL_MAP_CONTAINER_FLAG_XOR){
		out->_hash.data.size=ADDR(hash_fn)&(~SLL_MAP_CONTAINER_FLAG_XOR);
	}
	else{
		out->_hash.is_function=1;
		out->_hash.data.fn=hash_fn;
	}
	out->_eq_fn=equal_fn;
	out->_elem_count=0;
	out->_mask=0;
	do{
		SLL_RANDOM_BITS(out->_rng);
	} while (!out->_rng);
}



__SLL_EXTERNAL void sll_map_container_iter(sll_map_container_t* c,sll_container_callback_t callback){
	for (sll_size_t i=0;i<c->_elem_count;i++){
		if ((c->data+i)->_delete){
			continue;
		}
		callback((c->data+i)->value);
	}
}



__SLL_EXTERNAL void sll_map_container_iter_clear(sll_map_container_t* c,sll_container_callback_t callback){
	sll_map_container_iter(c,callback);
	sll_map_container_clear(c);
}



__SLL_EXTERNAL void sll_map_container_set(sll_map_container_t* c,const void* key,void* value){
	sll_size_t hash=_hash_key(c,key);
	sll_size_t index=_lookup_entry(c,hash,key);
	if (index!=0xffffffffffffffffull){
		SLL_ASSERT(index<=c->_mask);
		SLL_ASSERT(*(c->_indices+index)<c->_elem_count);
		(c->data+(*(c->_indices+index)))->value=value;
		return;
	}
	c->_elem_count++;
	sll_allocator_resize((void**)(&(c->data)),c->_elem_count*sizeof(sll_map_container_entry_t));
	(c->data+c->_elem_count-1)->key=key;
	(c->data+c->_elem_count-1)->value=value;
	(c->data+c->_elem_count-1)->_hash=hash;
	(c->data+c->_elem_count-1)->_delete=0;
	if (!c->_mask||c->_elem_count>=c->_mask-1){
		c->_mask=(c->_mask<<1)|1;
		c->_indices=sll_reallocate(c->_indices,(c->_mask+1)*sizeof(sll_size_t));
		sll_set_memory(c->_indices,(c->_mask+1)*sizeof(sll_size_t),0xff);
		for (sll_size_t i=0;i<c->_elem_count;i++){
			_index_element(c,i);
		}
	}
	else{
		_index_element(c,c->_elem_count-1);
	}
}

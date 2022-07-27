#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/memory.h>
#include <sll/platform/util.h>
#include <sll/types.h>



static sll_size_t _hash_key(const sll_map_container_t* container,const void* key){
	sll_size_t out=0;
	if (container->_hash.is_function){
		out=container->_hash.data.fn(key);
	}
	else if (container->_hash.data.size){
		SLL_UNIMPLEMENTED();
	}
	else{
		out=ADDR(key);
	}
	return ROTATE_BITS_RIGHT64(out*container->_rng,container->_rng&63);
}



static sll_size_t _lookup_entry(const sll_map_container_t* container,sll_size_t hash,const void* key){
	sll_size_t i=hash&container->_mask;
	for (sll_size_t j=0;j<container->_elem_count;j++){
		sll_size_t k=*(container->_indices+i);
		SLL_ASSERT(k==0xffffffffffffffffull||k<container->_elem_count);
		if (k!=0xffffffffffffffffull&&!(container->data+k)->_delete&&(container->data+k)->_hash==hash){
			const void* entry_key=(container->data+k)->key;
			if ((container->_hash.is_function&&container->_eq_fn(entry_key,key))||(!container->_hash.is_function&&container->_hash.data.size&&sll_compare_data(entry_key,key,container->_hash.data.size))||(!container->_hash.is_function&&!container->_hash.data.size&&entry_key==key)){
				return i;
			}
		}
		i=(i+1)&container->_mask;
	}
	return 0xffffffffffffffffull;
}



static void _index_element(const sll_map_container_t* container,sll_size_t i){
	sll_size_t j=(container->data+i)->_hash&container->_mask;
	while (container->_indices[j]!=0xffffffffffffffffull){
		j=(j+1)&container->_mask;
	}
	container->_indices[j]=i;
}



__SLL_EXTERNAL void sll_map_container_clear(sll_map_container_t* container){
	sll_allocator_release(container->data);
	sll_deallocate(container->_indices);
	container->data=NULL;
	container->_indices=NULL;
	container->_elem_count=0;
	container->_mask=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_container_contains(sll_map_container_t* container,const void* key){
	return (_lookup_entry(container,_hash_key(container,key),key)!=0xffffffffffffffffull);
}



__SLL_EXTERNAL void sll_map_container_deinit(sll_map_container_t* container){
	sll_allocator_release(container->data);
	sll_deallocate(container->_indices);
	sll_zero_memory(container,sizeof(sll_map_container_t));
}



__SLL_EXTERNAL void* sll_map_container_delete(sll_map_container_t* container,const void* key,void* default_){
	sll_size_t index=_lookup_entry(container,_hash_key(container,key),key);
	if (index==0xffffffffffffffffull){
		return default_;
	}
	sll_size_t data_index=*(container->_indices+index);
	SLL_ASSERT(data_index<container->_elem_count);
	SLL_ASSERT(index<=container->_mask);
	*(container->_indices+index)=0xffffffffffffffffull;
	(container->data+data_index)->_delete=1;
	return (container->data+data_index)->value;
}



__SLL_EXTERNAL sll_bool_t sll_map_container_filter(sll_map_container_t* container,sll_map_container_filter_callback_t callback,void* ctx){
	sll_bool_t out=0;
	for (sll_size_t i=0;i<container->_elem_count;i++){
		sll_map_container_entry_t* e=container->data+i;
		if (e->_delete||!callback(e->key,e->value,ctx)){
			continue;
		}
		out=1;
		e->_delete=1;
		sll_size_t j=0;
		while (*(container->_indices+j)!=i){
			j++;
		}
		SLL_ASSERT(j<=container->_mask);
		*(container->_indices+j)=0;
	}
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_map_container_get(sll_map_container_t* container,const void* key,void* default_){
	sll_size_t index=_lookup_entry(container,_hash_key(container,key),key);
	return (index!=0xffffffffffffffffull?(container->data+(*(container->_indices+index)))->value:default_);
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



__SLL_EXTERNAL void sll_map_container_iter(sll_map_container_t* container,sll_container_callback_t callback){
	for (sll_size_t i=0;i<container->_elem_count;i++){
		if ((container->data+i)->_delete){
			continue;
		}
		callback((container->data+i)->value);
	}
}



__SLL_EXTERNAL void sll_map_container_iter_clear(sll_map_container_t* container,sll_container_callback_t callback){
	sll_map_container_iter(container,callback);
	sll_map_container_clear(container);
}



__SLL_EXTERNAL void sll_map_container_set(sll_map_container_t* container,const void* key,void* value){
	sll_size_t hash=_hash_key(container,key);
	sll_size_t index=_lookup_entry(container,hash,key);
	if (index!=0xffffffffffffffffull){
		SLL_ASSERT(index<=container->_mask);
		SLL_ASSERT(*(container->_indices+index)<container->_elem_count);
		(container->data+(*(container->_indices+index)))->value=value;
		return;
	}
	container->_elem_count++;
	sll_allocator_resize((void**)(&(container->data)),container->_elem_count*sizeof(sll_map_container_entry_t));
	(container->data+container->_elem_count-1)->key=key;
	(container->data+container->_elem_count-1)->value=value;
	(container->data+container->_elem_count-1)->_hash=hash;
	(container->data+container->_elem_count-1)->_delete=0;
	if (!container->_mask||container->_elem_count>=container->_mask-1){
		container->_mask=(container->_mask<<1)|1;
		container->_indices=sll_reallocate(container->_indices,(container->_mask+1)*sizeof(sll_size_t));
		sll_set_memory(container->_indices,(container->_mask+1)*sizeof(sll_size_t),0xff);
		for (sll_size_t i=0;i<container->_elem_count;i++){
			_index_element(container,i);
		}
	}
	else{
		_index_element(container,container->_elem_count-1);
	}
}

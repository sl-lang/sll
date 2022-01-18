#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/version.h>
#include <stdint.h>



STATIC_STRING_SETUP;



static sll_cleanup_function_t _util_exit_table[MAX_CLEANUP_TABLE_SIZE];
static sll_bool_t _util_init=0;
static uint16_t _util_exit_table_size=0;
static sll_sandbox_flags_t _util_sandbox_flags=0;



__SLL_NO_RETURN void _force_exit(const sll_char_t* a,const sll_char_t* b,const sll_char_t* c,const sll_char_t* d,const sll_char_t* e){
	sll_file_flush(sll_stdout);
	sll_file_flush(sll_stderr);
	sll_file_descriptor_t fd=sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR);
	sll_platform_file_write(fd,a,sll_string_length_unaligned(a));
	sll_platform_file_write(fd,b,sll_string_length_unaligned(b));
	sll_platform_file_write(fd,c,sll_string_length_unaligned(c));
	if (d){
		sll_platform_file_write(fd,d,sll_string_length_unaligned(d));
		sll_platform_file_write(fd,e,sll_string_length_unaligned(e));
	}
	_force_exit_platform();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string(sll_string_table_t* st,sll_string_t* s,sll_bool_t d){
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,s)){
			if (d){
				sll_free_string(s);
			}
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=*s;
	return st->l-1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_add_string_object(sll_string_table_t* st,sll_object_t* v){
	SLL_ASSERT(SLL_OBJECT_GET_TYPE(v)==SLL_OBJECT_TYPE_STRING);
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,&(v->dt.s))){
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	v->t|=OBJECT_EXTERNAL_STRING;
	*(st->dt+st->l-1)=v->dt.s;
	return st->l-1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_compare_data(const void* a,const void* b,sll_size_t l){
	if (!l){
		return SLL_COMPARE_RESULT_EQUAL;
	}
	const sll_char_t* ap=(const sll_char_t*)a;
	const sll_char_t* bp=(const sll_char_t*)b;
	if (l<16){
		for (sll_size_t i=0;i<l;i++){
			if (*(ap+i)!=*(bp+i)){
				return (*(ap+i)<*(bp+i)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
			}
		}
		return SLL_COMPARE_RESULT_EQUAL;
	}
	if ((((uint64_t)ap)&7)&&(((uint64_t)bp)&7)){
		if ((((uint64_t)ap)&7)<(((uint64_t)bp)&7)){
			ap=(const sll_char_t*)b;
			bp=(const sll_char_t*)a;
		}
		SLL_UNIMPLEMENTED();
	}
	else if (((uint64_t)ap)&7){
		ap=(const sll_char_t*)b;
		bp=(const sll_char_t*)a;
	}
	const uint64_t* ap64=(const uint64_t*)ap;
	const uint64_t* bp64=(const uint64_t*)bp;
	SLL_ASSERT(!(((uint64_t)ap64)&7));
	do{
		if (*ap64!=*bp64){
			uint64_t av=ROTATE_BITS64(*ap64,32);
			uint64_t bv=ROTATE_BITS64(*bp64,32);
			av=((av&0xffff0000ffffull)<<16)|((av&0xffff0000ffff0000ull)>>16);
			bv=((bv&0xffff0000ffffull)<<16)|((bv&0xffff0000ffff0000ull)>>16);
			return ((((av&0xff00ff00ff00ffull)<<8)|((av&0xff00ff00ff00ff00ull)>>8))<(((bv&0xff00ff00ff00ffull)<<8)|((bv&0xff00ff00ff00ff00ull)>>8))?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		ap64++;
		bp64++;
		l-=8;
	} while (l>7);
	ap=(const sll_char_t*)ap64;
	bp=(const sll_char_t*)bp64;
	while (l){
		l--;
		if (*ap!=*bp){
			return (*ap<*bp?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		ap++;
		bp++;
	}
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_EXTERNAL void sll_copy_data(const void* s,sll_size_t l,void* d){
	const sll_char_t* a=(const sll_char_t*)s;
	sll_char_t* b=(sll_char_t*)d;
	if (l<16){
		while (l){
			*b=*a;
			a++;
			b++;
			l--;
		}
		return;
	}
	if (((uint64_t)d)&7){
		sll_size_t i=8-(((uint64_t)d)&7);
		a+=i;
		b+=i;
		l-=i;
		do{
			*(b-i)=*(a-i);
			i--;
		} while (i);
	}
	SLL_ASSERT(!(((uint64_t)b)&7));
	const uint64_t* ap=(const uint64_t*)a;
	uint64_t* bp=(uint64_t*)b;
	ASSUME_ALIGNED(bp,3,0);
	sll_size_t i=0;
	for (;i<(l>>3);i++){
		*(bp+i)=*(ap+i);
	}
	if (l&7){
		l=(l&7)<<3;
		*(bp+i)=((*(bp+i))&(0xffffffffffffffffull<<l))|((*(ap+i))&((1ull<<l)-1));
	}
}



__SLL_EXTERNAL void* sll_copy_string(const sll_char_t* s,void* d){
	if (!(*s)){
		return d;
	}
	sll_char_t* o=(sll_char_t*)d;
	while (((uint64_t)o)&7){
		*o=*s;
		s++;
		o++;
		if (!(*s)){
			return o;
		}
	}
	SLL_ASSERT(!(((uint64_t)o)&7));
	const uint64_t* sp=(const uint64_t*)s;
	uint64_t* op=(uint64_t*)o;
	ASSUME_ALIGNED(op,3,0);
	while (1){
		uint64_t v=((*sp)-0x101010101010101ull)&0x8080808080808080ull&(~(*sp));
		if (v){
			SLL_ASSERT(FIND_FIRST_SET_BIT(v)>6&&(FIND_FIRST_SET_BIT(v)&7)==7);
			v=FIND_FIRST_SET_BIT(v);
			o=(void*)(((uint64_t)op)+(v>>3));
			if (v>7){
				v-=7;
				*op=((*op)&(0xffffffffffffffffull<<v))|((*sp)&((1ull<<v)-1));
			}
			return o;
		}
		*op=*sp;
		sp++;
		op++;
	}
}



__SLL_EXTERNAL void sll_copy_string_null(const sll_char_t* s,void* d){
	*((sll_char_t*)sll_copy_string(s,d))=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_create_string(sll_string_table_t* st,const sll_char_t* dt,sll_string_length_t l){
	sll_string_t n;
	sll_string_from_pointer_length(dt,l,&n);
	for (sll_string_index_t i=0;i<st->l;i++){
		if (sll_string_equal(st->dt+i,&n)){
			sll_free_string(&n);
			return i;
		}
	}
	st->l++;
	st->dt=sll_reallocate(st->dt,st->l*sizeof(sll_string_t));
	*(st->dt+st->l-1)=n;
	return st->l-1;
}


__SLL_EXTERNAL void sll_deinit(void){
	if (!_util_init){
		return;
	}
	while (_util_exit_table_size){
		_util_exit_table_size--;
		_util_exit_table[_util_exit_table_size]();
	}
	const static_string_t*const* l=&__s_str_start;
	while (l<&__s_str_end){
		const static_string_t* k=*l;
		if (k){
			sll_free_string(k->p);
		}
		l++;
	}
	_gc_release_data();
	_file_release_std_streams();
	_log_release_data();
	_memory_release_data();
	sll_platform_reset_console();
	_util_sandbox_flags=0;
	_util_init=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_environment_variable(const sll_string_t* k,sll_string_t* o){
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		const sll_environment_variable_t* kv=*(sll_environment->dt+i);
		if (sll_string_equal(&(kv->k),k)){
			if (o){
				sll_string_clone(&(kv->v),o);
			}
			return 1;
		}
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_offset_t sll_get_location(const sll_assembly_data_t* a_dt,sll_instruction_index_t ii,sll_string_index_t* fp,sll_string_index_t* fn){
	if (ii>=a_dt->ic){
		*fp=0;
		return 0;
	}
	sll_instruction_index_t c=0;
	sll_file_offset_t o_ln=0;
	sll_string_index_t o_fn=SLL_MAX_STRING_INDEX;
	sll_string_index_t o_fp=0;
	for (sll_instruction_index_t i=0;i<a_dt->dbg.l;i++){
		c+=(a_dt->dbg.dt+i)->ii;
		if (c>ii){
			break;
		}
		if ((a_dt->dbg.dt+i)->ln&SLL_DEBUG_LINE_DATA_FLAG_FILE){
			o_fp=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
		else if ((a_dt->dbg.dt+i)->ln&SLL_DEBUG_LINE_DATA_FLAG_FUNC){
			o_fn=(sll_string_index_t)SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
		else{
			o_ln=SLL_DEBUG_LINE_DATA_GET_DATA(a_dt->dbg.dt+i);
		}
	}
	*fp=o_fp;
	*fn=o_fn;
	return o_ln+1;
}


__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_get_sandbox_flag(sll_sandbox_flags_t f){
	return !!(_util_sandbox_flags&f);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_get_sandbox_flags(void){
	return _util_sandbox_flags;
}



__SLL_EXTERNAL void sll_init(void){
	if (_util_init){
		return;
	}
	sll_platform_setup_console();
	_file_init_std_streams();
	_init_platform();
	const static_string_t*const* l=&__s_str_start;
	while (l<&__s_str_end){
		const static_string_t* k=*l;
		if (k){
			sll_string_from_pointer_length(k->dt,k->dtl,k->p);
		}
		l++;
	}
}



__SLL_EXTERNAL void sll_remove_environment_variable(const sll_string_t* k){
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		sll_environment_variable_t* kv=(sll_environment_variable_t*)(*(sll_environment->dt+i));
		if (sll_string_equal(k,&(kv->k))){
			sll_platform_remove_environment_variable(k->v);
			sll_free_string((sll_string_t*)(&(kv->k)));
			sll_free_string((sll_string_t*)(&(kv->v)));
			sll_deallocate(kv);
			i++;
			while (i<sll_environment->l){
				*(((const sll_environment_variable_t**)(sll_environment->dt))+i-1)=*(sll_environment->dt+i);
			}
			(*((sll_array_length_t*)(&(sll_environment->l))))--;
			*((const sll_environment_variable_t*const**)(&(sll_environment->dt)))=sll_reallocate((void*)(sll_environment->dt),sll_environment->l*sizeof(sll_environment_variable_t*));
			return;
		}
	}
}



__SLL_EXTERNAL void sll_set_environment_variable(const sll_string_t* k,const sll_string_t* v){
	sll_platform_set_environment_variable(k->v,v->v);
	for (sll_array_length_t i=0;i<sll_environment->l;i++){
		sll_environment_variable_t* kv=(sll_environment_variable_t*)(*(sll_environment->dt+i));
		if (sll_string_equal(k,&(kv->k))){
			sll_free_string((sll_string_t*)(&(kv->v)));
			sll_string_clone(v,(sll_string_t*)(&(kv->v)));
			return;
		}
	}
	(*((sll_array_length_t*)(&(sll_environment->l))))++;
	*((const sll_environment_variable_t*const**)(&(sll_environment->dt)))=sll_reallocate((void*)(sll_environment->dt),sll_environment->l*sizeof(sll_environment_variable_t*));
	sll_environment_variable_t* n=sll_allocate(sizeof(sll_environment_variable_t));
	sll_string_clone(k,(sll_string_t*)(&(n->k)));
	sll_string_clone(v,(sll_string_t*)(&(n->v)));
	*(((const sll_environment_variable_t**)(sll_environment->dt))+sll_environment->l-1)=n;
}



__SLL_EXTERNAL void sll_register_cleanup(sll_cleanup_function_t f){
	SLL_ASSERT(_util_exit_table_size<MAX_CLEANUP_TABLE_SIZE);
	_util_exit_table[_util_exit_table_size]=f;
	_util_exit_table_size++;
}



__SLL_EXTERNAL void sll_set_memory(void* p,sll_size_t l,sll_char_t v){
	sll_char_t* o=(sll_char_t*)p;
	if (l<16){
		while (l){
			*o=v;
			o++;
			l--;
		}
		return;
	}
	if (((uint64_t)o)&7){
		sll_size_t i=8-(((uint64_t)o)&7);
		l-=i;
		do{
			*o=v;
			o++;
			i--;
		} while (i);
	}
	SLL_ASSERT(!(((uint64_t)o)&7));
	uint64_t* op=(uint64_t*)o;
	ASSUME_ALIGNED(op,3,0);
	sll_size_t i=0;
	uint64_t v64=0x101010101010101ull*v;
	for (;i<(l>>3);i++){
		*(op+i)=v64;
	}
	if (l&7){
		l=(l&7)<<3;
		*(op+i)=((*(op+i))&(0xffffffffffffffffull<<l))|(v64&((1ull<<l)-1));
	}
}



__SLL_EXTERNAL sll_sandbox_flags_t sll_set_sandbox_flags(sll_sandbox_flags_t f){
	_util_sandbox_flags|=f;
	return _util_sandbox_flags;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_version_t sll_version(void){
	return SLL_VERSION;
}



__SLL_EXTERNAL void sll_zero_memory(void* p,sll_size_t l){
	sll_char_t* o=(sll_char_t*)p;
	if (l<16){
		while (l){
			*o=0;
			o++;
			l--;
		}
		return;
	}
	if (((uint64_t)o)&7){
		sll_size_t i=8-(((uint64_t)o)&7);
		l-=i;
		do{
			*o=0;
			o++;
			i--;
		} while (i);
	}
	SLL_ASSERT(!(((uint64_t)o)&7));
	uint64_t* op=(uint64_t*)o;
	ASSUME_ALIGNED(op,3,0);
	sll_size_t i=0;
	for (;i<(l>>3);i++){
		*(op+i)=0;
	}
	if (l&7){
		*(op+i)&=0xffffffffffffffffull<<((l&7)<<3);
	}
}

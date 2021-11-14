#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/runtime_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdio.h>



static sll_file_t** _file_fl=NULL;
static sll_handle_t _file_fll=0;
static sll_handle_type_t _file_ht=SLL_HANDLE_UNKNOWN_TYPE;
static sll_handle_descriptor_t _file_type;



static sll_bool_t _free_file(sll_handle_t h){
	if (h>=_file_fll){
		return 0;
	}
	sll_file_t* f=*(_file_fl+h);
	if (!f){
		return 0;
	}
	sll_file_close(f);
	sll_deallocate(f);
	*(_file_fl+h)=NULL;
	if (h==_file_fll-1){
		do{
			_file_fll--;
		} while (_file_fll&&!(_file_fl+_file_fll-1));
		if (_file_fll){
			void* tmp=sll_reallocate(_file_fl,_file_fll*sizeof(sll_file_t*));
			SLL_ASSERT(tmp);
			_file_fl=tmp;
		}
		else{
			sll_deallocate(_file_fl);
			_file_fl=NULL;
		}
	}
	return 1;
}



static void _file_destructor(sll_handle_t h){
	if (h==SLL_HANDLE_FREE){
		SLL_ASSERT(!_file_fll);
		_file_ht=SLL_HANDLE_UNKNOWN_TYPE;
		return;
	}
	_free_file(h);
}



__API_FUNC(file_close){
	if (a->t!=_file_ht||a->h>=_file_fll){
		return 0;
	}
	return _free_file(a->h);
}



__API_FUNC(file_open){
	SLL_INIT_HANDLE_DATA(out);
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH){
		return;
	}
	sll_file_flags_t ff=SLL_FILE_FLAG_READ;
	sll_bool_t chk_e=0;
	if (b){
		for (sll_string_length_t i=0;i<b->l;i++){
			if (b->v[i]=='a'||b->v[i]=='A'){
				ff=SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND;
				chk_e=0;
			}
			else if (b->v[i]=='r'||b->v[i]=='R'){
				ff=SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND;
				chk_e=0;
			}
			else if (b->v[i]=='w'||b->v[i]=='W'){
				ff=SLL_FILE_FLAG_WRITE;
				chk_e=0;
			}
			else if (b->v[i]=='x'||b->v[i]=='X'){
				ff=SLL_FILE_FLAG_WRITE;
				chk_e=1;
			}
		}
	}
	if (chk_e&&sll_platform_path_exists(a->v)){
		return;
	}
	sll_file_t f;
	if (!sll_file_open(a->v,ff,&f)){
		return;
	}
	sll_handle_t i=0;
	while (i<_file_fll){
		if (!(_file_fl+i)){
			goto _found_index;
		}
		i++;
	}
	_file_fll++;
	void* tmp=sll_reallocate(_file_fl,_file_fll*sizeof(sll_file_t));
	if (!tmp){
		sll_file_close(&f);
		return;
	}
	_file_fl=tmp;
_found_index:;
	sll_file_t* n=sll_allocate(sizeof(sll_file_t));
	sll_copy_data((void*)(&f),sizeof(sll_file_t),(void*)n);
	*(_file_fl+i)=n;
	if (_file_ht==SLL_HANDLE_UNKNOWN_TYPE){
		SLL_ASSERT(sll_current_runtime_data);
		_file_ht=sll_create_handle(sll_current_runtime_data->hl,&_file_type);
	}
	out->t=_file_ht;
	out->h=i;
}



__API_FUNC(file_write){
	sll_file_t* f=NULL;
	if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT){
		if (a->dt.i==-2){
			SLL_ASSERT(sll_current_runtime_data);
			f=sll_current_runtime_data->out;
		}
		else if (a->dt.i==-3){
			f=sll_stderr;
		}
		else{
			return 0;
		}
	}
	else if (a->dt.h.t==_file_ht&&a->dt.h.h<_file_fll){
		f=*(_file_fl+a->dt.h.h);
		if (!f){
			return 0;
		}
	}
	sll_string_t s;
	sll_object_to_string(b,bc,&s);
	sll_size_t o=sll_file_write(f,s.v,s.l*sizeof(sll_char_t));
	sll_deinit_string(&s);
	return o*sizeof(sll_char_t);
}



static sll_handle_descriptor_t _file_type={
	SLL_HANDLE_DESCRIPTOR_HEADER("sll_file_handle"),
	NULL,
	_file_destructor
};

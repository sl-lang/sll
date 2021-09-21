#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/path.h>
#include <sll/api/string.h>
#include <sll/assembly.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct __FILE{
	FILE* h;
} file_t;



static file_t* _file_fl=NULL;
static sll_handle_t _file_fll=0;
static sll_handle_type_t _file_ht=SLL_UNKNOWN_HANDLE_TYPE;
static sll_handle_descriptor_t _file_type;



static uint8_t _free_file(sll_handle_t h){
	if (h>=_file_fll){
		return 0;
	}
	file_t* f=_file_fl+h;
	if (!f->h){
		return 0;
	}
	fclose(f->h);
	f->h=NULL;
	if (h==_file_fll-1){
		do{
			_file_fll--;
		} while (_file_fll&&!(_file_fl+_file_fll-1)->h);
		if (_file_fll){
			void* tmp=realloc(_file_fl,_file_fll*sizeof(file_t));
			SLL_ASSERT(tmp);
			_file_fl=tmp;
		}
		else{
			free(_file_fl);
			_file_fl=NULL;
		}
	}
	return 1;
}



static void _file_destructor(sll_handle_t h){
	if (h==SLL_HANDLE_FREE){
		SLL_ASSERT(!_file_fll);
		_file_ht=SLL_UNKNOWN_HANDLE_TYPE;
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
	SLL_ZERO_HANDLE(out);
	if (a->l>SLL_API_MAX_FILE_PATH_LENGTH){
		return;
	}
	const char* m="rb";
	if (b){
		for (sll_string_length_t i=0;i<b->l;i++){
			if (b->v[i]=='a'||b->v[i]=='A'){
				m="ab";
			}
			else if (b->v[i]=='w'||b->v[i]=='W'){
				m="wb";
			}
			else if (b->v[i]=='x'||b->v[i]=='X'){
				m="xb";
			}
		}
	}
	FILE* h=fopen((char*)a->v,m);// lgtm [cpp/path-injection]
	if (!h){
		return;
	}
	sll_handle_t i=0;
	while (i<_file_fll){
		if (!(_file_fl+i)->h){
			goto _found_index;
		}
		i++;
	}
	_file_fll++;
	void* tmp=realloc(_file_fl,_file_fll*sizeof(file_t));
	if (!tmp){
		fclose(h);
		return;
	}
	_file_fl=tmp;
_found_index:
	(_file_fl+i)->h=h;
	if (_file_ht==SLL_UNKNOWN_HANDLE_TYPE){
		SLL_ASSERT(sll_current_runtime_data);
		_file_ht=sll_create_handle(sll_current_runtime_data->hl,&_file_type);
	}
	out->t=_file_ht;
	out->h=i;
}



__API_FUNC(file_write){
	FILE* fh=NULL;
	if (SLL_RUNTIME_OBJECT_GET_TYPE(a)==SLL_RUNTIME_OBJECT_TYPE_INT){
		if (a->dt.i==-2){
			fh=stdout;
		}
		else if (a->dt.i==-3){
			fh=stderr;
		}
		else{
			return 0;
		}
	}
	else if (a->dt.h.t==_file_ht&&a->dt.h.h<_file_fll){
		fh=(_file_fl+a->dt.h.h)->h;
		if (!fh){
			return 0;
		}
	}
	else{
		return 0;
	}
	sll_string_t s;
	sll_object_to_string(b,bc,&s);
	fwrite(s.v,sizeof(sll_char_t),s.l,fh);
	free(s.v);
	return s.l;
}



static sll_handle_descriptor_t _file_type={
	SLL_HANDLE_DESCRIPTOR_HEADER("sll_file_handle"),
	NULL,
	_file_destructor
};

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/assembly.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



#define SETUP_HANDLE \
	do{ \
		if (_file_ht==SLL_UNKNOWN_HANDLE_TYPE){ \
			SLL_ASSERT(sll_current_runtime_data); \
			_file_ht=sll_create_handle(sll_current_runtime_data->hl,(sll_char_t*)"sll_file_handle",_file_destructor); \
		} \
	} while (0)



typedef struct __FILE{
	FILE* h;
} file_t;



static file_t* _file_fl=NULL;
static sll_handle_t _file_fll=0;
static sll_handle_type_t _file_ht=SLL_UNKNOWN_HANDLE_TYPE;



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
	if (!ac){
		SLL_RETURN_ZERO;
	}
	SETUP_HANDLE;
	const sll_runtime_object_t* v=*a;
	if (v->t!=SLL_RUNTIME_OBJECT_TYPE_HANDLE||v->dt.h.t!=_file_ht||v->dt.h.h>=_file_fll){
		SLL_RETURN_ZERO;
	}
	if (_free_file(v->dt.h.h)){
		SLL_RETURN_ONE;
	}
	SLL_RETURN_ZERO;
}



__API_FUNC(file_open){
	if (!ac){
		SLL_RETURN_ZERO_HANDLE;
	}
	SETUP_HANDLE;
	const sll_runtime_object_t* fp=*a;
	if (fp->t!=SLL_RUNTIME_OBJECT_TYPE_STRING||fp->dt.s.l>SLL_API_MAX_FILE_PATH_LENGTH){
		SLL_RETURN_ZERO_HANDLE;
	}
	const char* m="rb";
	if (ac>1&&(*(a+1))->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		sll_string_t s=(*(a+1))->dt.s;
		for (sll_string_length_t i=0;i<s.l;i++){
			if (s.v[i]=='a'||s.v[i]=='A'){
				m="ab";
			}
			else if (s.v[i]=='w'||s.v[i]=='W'){
				m="wb";
			}
			else if (s.v[i]=='x'||s.v[i]=='X'){
				m="xb";
			}
		}
	}
	FILE* h=fopen((char*)fp->dt.s.v,m);// lgtm [cpp/path-injection]
	if (!h){
		SLL_RETURN_ZERO_HANDLE;
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
		SLL_RETURN_ZERO_HANDLE;
	}
	_file_fl=tmp;
_found_index:
	(_file_fl+i)->h=h;
	return SLL_FROM_HANDLE(_file_ht,i);
}



__API_FUNC(file_write){
	if (ac<2){
		SLL_RETURN_ZERO;
	}
	SETUP_HANDLE;
	const sll_runtime_object_t* v=*a;
	FILE* fh=NULL;
	if (v->t==SLL_RUNTIME_OBJECT_TYPE_INT){
		if (v->dt.i==-2){
			fh=stdout;
		}
		else if (v->dt.i==-3){
			fh=stderr;
		}
		else{
			SLL_RETURN_ZERO;
		}
	}
	else if (v->t==SLL_RUNTIME_OBJECT_TYPE_HANDLE&&v->dt.h.t==_file_ht&&v->dt.h.h<_file_fll){
		fh=(_file_fl+v->dt.h.h)->h;
		if (!fh){
			SLL_RETURN_ZERO;
		}
	}
	else{
		SLL_RETURN_ZERO;
	}
	sll_string_t s;
	sll_object_to_string(a+1,ac-1,&s);
	fwrite(s.v,sizeof(sll_char_t),s.l,fh);
	free(s.v);
	return SLL_FROM_INT(s.l);
}

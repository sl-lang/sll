#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct __FILE{
	FILE* h;
} file_t;



static file_t* _file_fl=NULL;
static uint16_t _file_fll=0;



__API_FUNC(file_close){
	if (!ac){
		SLL_RETURN_ZERO;
	}
	const sll_runtime_object_t* v=*a;
	if (v->t!=SLL_RUNTIME_OBJECT_TYPE_INT||v->dt.i<0||v->dt.i>=_file_fll){
		SLL_RETURN_ZERO;
	}
	file_t* f=_file_fl+v->dt.i;
	if (f->h){
		fclose(f->h);
		f->h=NULL;
		if (v->dt.i==_file_fll-1){
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
		SLL_RETURN_ONE;
	}
	SLL_RETURN_ZERO;
}



__API_FUNC(file_open){
	if (_file_fll==SLL_API_MAX_OPEN_FILES||!ac){
		SLL_RETURN_ZERO;
	}
	const sll_runtime_object_t* fp=*a;
	if (fp->t!=SLL_RUNTIME_OBJECT_TYPE_STRING||fp->dt.s.l>SLL_API_MAX_FILE_PATH_LENGTH){
		SLL_RETURN_ZERO;
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
		SLL_RETURN_ZERO;
	}
	uint16_t i=0;
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
		SLL_RETURN_ZERO;
	}
	_file_fl=tmp;
_found_index:
	(_file_fl+i)->h=h;
	return SLL_FROM_INT(i);
}



__API_FUNC(file_write){
	if (ac<2){
		SLL_RETURN_ZERO;
	}
	const sll_runtime_object_t* v=*a;
	if (v->t!=SLL_RUNTIME_OBJECT_TYPE_INT){
		SLL_RETURN_ZERO;
	}
	FILE* fh=NULL;
	if (v->dt.i==-2){
		fh=stdout;
	}
	else if (v->dt.i==-3){
		fh=stderr;
	}
	else if (v->dt.i<0||v->dt.i>=_file_fll){
		SLL_RETURN_ZERO;
	}
	else{
		fh=(_file_fl+v->dt.i)->h;
		if (!fh){
			SLL_RETURN_ZERO;
		}
	}
	sll_string_t s;
	sll_object_to_string(a+1,ac-1,&s);
	fwrite(s.v,sizeof(sll_char_t),s.l,fh);
	free(s.v);
	return SLL_FROM_INT(s.l);
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct __FILE{
	FILE* h;
} file_t;



file_t* _file_fl=NULL;
uint16_t _file_fll=0;



__API_FUNC(file_close){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
	if (!ac||a->t!=SLL_RUNTIME_OBJECT_TYPE_INT||a->dt.i<0||a->dt.i>=_file_fll){
		return;
	}
	file_t* f=_file_fl+a->dt.i;
	if (f->h){
		fclose(f->h);
		f->h=NULL;
		if (a->dt.i==_file_fll-1){
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
		o->dt.i=1;
	}
}



__API_FUNC(file_open){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	if (_file_fll==SLL_API_MAX_OPEN_FILES||!ac||a->t!=SLL_RUNTIME_OBJECT_TYPE_STRING||a->dt.s->l>SLL_API_MAX_FILE_PATH_LENGTH){
		return;
	}
	const char* m="rb";
	if (ac>1&&(a+1)->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		sll_string_t* s=(a+1)->dt.s;
		for (sll_string_length_t i=0;i<s->l;i++){
			if (s->v[i]=='a'||s->v[i]=='A'){
				m="ab";
			}
			else if (s->v[i]=='w'||s->v[i]=='W'){
				m="wb";
			}
			else if (s->v[i]=='x'||s->v[i]=='X'){
				m="xb";
			}
		}
	}
	FILE* h=fopen((char*)a->dt.s->v,m);// lgtm [cpp/path-injection]
	if (!h){
		return;
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
		return;
	}
	_file_fl=tmp;
_found_index:
	(_file_fl+i)->h=h;
	o->dt.i=i;
}
#define _SLL_ASSERT_STR_(x) #x
#define _SLL_ASSERT_STR(x) _SLL_ASSERT_STR_(x)



__API_FUNC(file_write){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
	if (ac<2||a->t!=SLL_RUNTIME_OBJECT_TYPE_INT){
		return;
	}
	FILE* fh=NULL;
	if (a->dt.i==-2){
		fh=stdout;
	}
	else if (a->dt.i==-3){
		fh=stderr;
	}
	else if (a->dt.i<0||a->dt.i>=_file_fll){
		return;
	}
	else{
		fh=(_file_fl+a->dt.i)->h;
		if (!fh){
			return;
		}
	}
	sll_string_t* s=sll_object_to_string(a+1,ac-1);
	fwrite(s->v,sizeof(sll_char_t),s->l,fh);
	o->dt.i+=s->l;
	sll_string_release(s);
}

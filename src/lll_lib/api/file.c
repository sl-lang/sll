#include <lll/_lll_internal.h>
#include <lll/api.h>
#include <lll/common.h>
#include <lll/string.h>
#include <lll/constants.h>
#include <lll/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



typedef struct __FILE{
	FILE* h;
} file_t;



file_t* _file_fl=NULL;
uint16_t _file_fll=0;



__API_FUNC(lll_api_open_file){
	if (_file_fll==LLL_API_MAX_OPEN_FILES||!ac||a->t!=LLL_RUNTIME_OBJECT_TYPE_STRING||a->dt.s->l>LLL_API_MAX_FILE_PATH_LENGTH){
		o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
		o->dt.i=LLL_API_INVALID_FILE_HANDLE;
		return;
	}
	const char* m="rb";
	if (ac>1){
		if ((a+1)->t==LLL_RUNTIME_OBJECT_TYPE_INT){
			if ((a+1)->dt.i==LLL_API_FILE_APPEND){
				m="ab";
			}
			else if ((a+1)->dt.i==LLL_API_FILE_WRITE){
				m="wb";
			}
		}
		else if ((a+1)->t==LLL_RUNTIME_OBJECT_TYPE_STRING){
			if (lll_string_compare_ignore_case_ascii((a+1)->dt.s,"a")||lll_string_compare_ignore_case_ascii((a+1)->dt.s,"ab")){
				m="ab";
			}
			else if (lll_string_compare_ignore_case_ascii((a+1)->dt.s,"w")||lll_string_compare_ignore_case_ascii((a+1)->dt.s,"wb")){
				m="wb";
			}
		}
	}
	lll_string_length_t sz=lll_string_length_as_bytes(a->dt.s);
	char* bf=malloc((sz+1)*sizeof(char));
	lll_string_to_bytes(a->dt.s,(lll_buffer_t)bf);
	*(bf+sz)=0;
	FILE* h=fopen(bf,m);
	free(bf);
	if (!h){
		o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
		o->dt.i=LLL_API_INVALID_FILE_HANDLE;
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
	_file_fl=realloc(_file_fl,_file_fll*sizeof(file_t));
_found_index:
	(_file_fl+i)->h=h;
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=i;
}



__API_FUNC(lll_api_close_file){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
	if (!ac||a->t!=LLL_RUNTIME_OBJECT_TYPE_INT||a->dt.i<0||a->dt.i>=_file_fll){
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
				_file_fl=realloc(_file_fl,_file_fll*sizeof(file_t));
			}
			else{
				free(_file_fl);
				_file_fl=NULL;
			}
		}
		o->dt.i=1;
	}
}



__API_FUNC(lll_api_write_file){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
	if (ac<2||a->t!=LLL_RUNTIME_OBJECT_TYPE_INT||a->dt.i<0||a->dt.i>=LLL_API_MAX_OPEN_FILES){
		return;
	}
	file_t* f=_file_fl+a->dt.i;
	if (f->h){
		if ((a+1)->t!=LLL_RUNTIME_OBJECT_TYPE_STRING){
			ASSERT(!"Unimplemented");
		}
		lll_string_length_t sz=lll_string_length_as_bytes((a+1)->dt.s);
		uint8_t* bf=malloc(sz*sizeof(uint8_t));
		lll_string_to_bytes((a+1)->dt.s,bf);
		fwrite(bf,sizeof(uint8_t),sz,f->h);
		free(bf);
		o->dt.i=sz;
	}
}



INTERNAL_FUNCTION("file_open",lll_api_open_file)
INTERNAL_FUNCTION("file_close",lll_api_close_file)
INTERNAL_FUNCTION("file_write",lll_api_write_file)

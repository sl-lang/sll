#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <intrin.h>
#else
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
#include <run_tests.h>
#include <sll/__coverage.h>
#include <sll_standalone.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#ifdef _MSC_VER
#pragma intrinsic(_BitScanForward64)
static __inline __forceinline unsigned int FIND_FIRST_SET_BIT(unsigned __int64 m){
	unsigned long o;
	_BitScanForward64(&o,m);
	return o;
}
#define POPCNT(m) __popcnt64((m))
#else
#define FIND_FIRST_SET_BIT(m) (__builtin_ffsll((m))-1)
#define POPCNT(m) __builtin_popcountll((m))
#endif



uint8_t c_st[COMPILER_STACK_SIZE];
char e_fp[4096];
char t_fp[4096];
char c_fp[4096];



void list_files(char* fp,uint16_t fpl,file_callback_t cb,test_result_t* o){
#ifdef _MSC_VER
	WIN32_FIND_DATAA dt;
	fp[fpl]='*';
	fp[fpl+1]=0;
	HANDLE fh=FindFirstFileA(fp,&dt);
	if (fh!=INVALID_HANDLE_VALUE){
		do{
			if (!(dt.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)){
				uint16_t i=0;
				while (*(dt.cFileName+i)){
					*(fp+fpl+i)=*(dt.cFileName+i);
					i++;
				}
				*(fp+fpl+i)=0;
				cb(fp,o);
			}
		} while (FindNextFileA(fh,&dt));
		FindClose(fh);
	}
#else
	DIR* d=opendir(fp);
	if (d){
		struct dirent* dt;
		while ((dt=readdir(d))){
			if (dt->d_type==DT_REG){
				uint16_t i=0;
				while (*(dt->d_name+i)){
					*(fp+fpl+i)=*(dt->d_name+i);
					i++;
				}
				*(fp+fpl+i)=0;
				cb(fp,o);
			}
		}
		closedir(d);
	}
#endif
}



uint8_t execute_test(uint8_t id){
	if (id==TEST_ID_PARSE){
		sll_internal_function_table_t i_ft;
		sll_create_internal_function_table(&i_ft);
		sll_register_standard_internal_functions(&i_ft);
		FILE* f=fopen(t_fp,"rb");// lgtm [cpp/path-injection]
		if (!f){
			return 1;
		}
		sll_input_data_stream_t is;
		sll_stream_create_input_from_file(f,&is);
		sll_compilation_data_t c_dt;
		sll_init_compilation_data((sll_char_t*)"<internal>",&is,&c_dt);
		sll_set_compilation_data_stack(&c_dt,c_st,COMPILER_STACK_SIZE);
		sll_error_t e;
		if (!sll_parse_all_objects(&c_dt,&i_ft,NULL,&e)){
			fwrite((void*)&e,sizeof(sll_error_t),1,stdout);
		}
		sll_free_compilation_data(&c_dt);
		fclose(f);
		goto _save_coverage;
	}
	return 1;
_save_coverage:;
	FILE* c_f=fopen(c_fp,"wb");
	if (!c_f){
		return 1;
	}
	fwrite(__coverage_map,sizeof(uint64_t),__coverage_map_length,c_f);
	fclose(c_f);
	return 0;
}



int8_t execute_subprocess(uint8_t id,char* bf,uint32_t* o){
	*o=0;
#ifdef _MSC_VER
	HANDLE rh=INVALID_HANDLE_VALUE;
	HANDLE wh=INVALID_HANDLE_VALUE;
	SECURITY_ATTRIBUTES sa={0};
	sa.nLength=sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor=NULL;
	sa.bInheritHandle=TRUE;
	if (!CreatePipe(&rh,&wh,&sa,0)||!SetHandleInformation(rh,HANDLE_FLAG_INHERIT,0)){
		return -1;
	}
	STARTUPINFOA si={0};
	si.cb=sizeof(STARTUPINFOA);
	si.dwFlags=STARTF_USESTDHANDLES;
	si.hStdInput=GetStdHandle(STD_INPUT_HANDLE);
	si.hStdOutput=wh;
	si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
	PROCESS_INFORMATION pi;
	uint16_t i=0;
	while (e_fp[i]){
		i++;
	}
	e_fp[i]=' ';
	e_fp[i+1]=id+1;
	e_fp[i+2]=0;
	if (!CreateProcessA(NULL,e_fp,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi)){
		return -1;
	}
	e_fp[i]=0;
	CloseHandle(wh);
	char tmp[4097];
	while (1){
		DWORD l=0;
		if (!ReadFile(rh,tmp,4096,&l,NULL)&&l){
			return -1;
		}
		if (!l){
			break;
		}
		for (DWORD j=0;j<l;j++){
			*(bf+(*o)+j)=*(tmp+j);
		}
		(*o)+=l;
	}
	CloseHandle(rh);
	*(bf+(*o))=0;
	DWORD ec;
	if (WaitForSingleObject(pi.hProcess,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(pi.hProcess,&ec)){
		return -1;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return !!ec;
#else
	int p[2];
	if (pipe(p)==-1){
		return -1;
	}
	pid_t f=fork();
	if (f==-1){
		close(p[1]);
		return -1;
	}
	if (f==0){
		while (dup2(p[1],STDOUT_FILENO)==-1&&errno==EINTR);
		close(p[0]);
		close(p[1]);
		exit(execute_test(id));
	}
	close(p[1]);
	char tmp[4096];
	while (1){
		ssize_t l=read(p[0],tmp,4096);
		if (l==-1){
			if (errno==EINTR){
				continue;
			}
			close(p[0]);
			return -1;
		}
		if (!l){
			break;
		}
		for (ssize_t i=0;i<l;i++){
			*(bf+(*o)+i)=*(tmp+i);
		}
		(*o)+=l;
	}
	close(p[0]);
	int st;
	if (waitpid(-1,&st,0)==-1){
		return -1;
	}
	return (!WIFEXITED(st)||WEXITSTATUS(st));
#endif
}



void _parse_json_str(json_parser_state_t* p,string_32bit_t* o){
	o->l=1;
	o->v=malloc(sizeof(char));
	*(o->v)=0;
	char c=**p;
	(*p)++;
	while (c!='\"'){
		if (c!='\\'){
			o->l++;
			o->v=realloc(o->v,o->l*sizeof(char));
			*(o->v+o->l-2)=c;
		}
		else{
			c=**p;
			(*p)++;
			if (c=='/'||c=='\\'||c=='\''||c=='\"'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=c;
			}
			else if (c=='b'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=8;
			}
			else if (c=='f'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=12;
			}
			else if (c=='n'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=10;
			}
			else if (c=='r'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=13;
			}
			else if (c=='t'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=9;
			}
			else if (c=='v'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=11;
			}
			else if (c=='x'){
				char a=**p;
				(*p)++;
				char b=**p;
				(*p)++;
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(char));
				*(o->v+o->l-2)=((a>47&&a<58?a-48:(a>64&&a<71?a-55:a-87))<<4)|(b>47&&b<58?b-48:(b>64&&b<71?b-55:b-87));
			}
			else{
				printf("Unknown Escape: \\%c\n",c);
			}
		}
		c=**p;
		(*p)++;
	}
	o->l--;
	*(o->v+o->l)=0;
}



uint8_t parse_json(json_parser_state_t* p,json_object_t* o){
	char c=**p;
	(*p)++;
	while (c==' '||c=='\t'||c=='\n'||c=='\r'){
		c=**p;
		(*p)++;
	}
	if (c=='{'){
		o->t=JSON_OBJECT_TYPE_MAP;
		o->dt.m.l=0;
		o->dt.m.dt=NULL;
		while (1){
			c=**p;
			(*p)++;
			while (c!='\"'){
				if (c=='}'){
					return 0;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return 1;
				}
				c=**p;
				(*p)++;
			}
			o->dt.m.l++;
			o->dt.m.dt=realloc(o->dt.m.dt,o->dt.m.l*sizeof(json_map_keypair_t));
			json_map_keypair_t* k=o->dt.m.dt+o->dt.m.l-1;
			_parse_json_str(p,&(k->k));
			c=**p;
			(*p)++;
			while (c!=':'){
				c=**p;
				(*p)++;
			}
			if (parse_json(p,&(k->v))){
				return 1;
			}
			c=**p;
			(*p)++;
			while (c!=','){
				if (c=='}'){
					return 0;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return 1;
				}
				c=**p;
				(*p)++;
			}
		}
	}
	if (c=='['){
		o->t=JSON_OBJECT_TYPE_ARRAY;
		o->dt.a.l=0;
		o->dt.a.dt=NULL;
		while (c==' '||c=='\t'||c=='\n'||c=='\r'){
			c=**p;
			(*p)++;
		}
		if (c==']'){
			return 0;
		}
		while (1){
			o->dt.a.l++;
			o->dt.a.dt=realloc(o->dt.a.dt,o->dt.a.l*sizeof(json_object_t));
			if (parse_json(p,o->dt.a.dt+o->dt.a.l-1)){
				return 1;
			}
			c=**p;
			(*p)++;
			while (c!=','){
				if (c==']'){
					return 0;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return 1;
				}
				c=**p;
				(*p)++;
			}
		}
	}
	if (c=='\"'){
		o->t=JSON_OBJECT_TYPE_STRING;
		_parse_json_str(p,&(o->dt.s));
		return 0;
	}
	if (c=='t'&&**p=='r'&&*((*p)+1)=='u'&&*((*p)+2)=='e'){
		(*p)+=3;
		o->t=JSON_OBJECT_TYPE_TRUE;
		return 0;
	}
	if (c=='f'&&**p=='a'&&*((*p)+1)=='l'&&*((*p)+2)=='s'&&*((*p)+3)=='e'){
		(*p)+=4;
		o->t=JSON_OBJECT_TYPE_FALSE;
		return 0;
	}
	if (c=='n'&&**p=='u'&&*((*p)+1)=='l'&&*((*p)+2)=='l'){
		(*p)+=3;
		o->t=JSON_OBJECT_TYPE_NULL;
		return 0;
	}
	if ((c<48||c>57)&&c!='.'&&c!='e'&&c!='E'&&c!='-'&&c!='+'){
		return 1;
	}
	int8_t s=1;
	if (c=='+'){
		c=**p;
		(*p)++;
	}
	else if (c=='-'){
		s=-1;
		c=**p;
		(*p)++;
	}
	double v=0;
	while (c>47&&c<58){
		v=v*10+(c-48);
		c=**p;
		(*p)++;
	}
	if (c!='.'&&c!='e'&&c!='E'){
		(*p)--;
		o->t=JSON_OBJECT_TYPE_INTEGER;
		o->dt.i=(int64_t)(v*s);
		return 0;
	}
	if (c=='.'){
		double pw=0.1;
		c=**p;
		(*p)++;
		while (c>47&&c<58){
			v+=pw*(c-48);
			pw*=0.1;
			c=**p;
			(*p)++;
		}
	}
	if (c=='e'||c=='E'){
		c=**p;
		(*p)++;
		int8_t pw_s=1;
		if (c=='+'){
			c=**p;
			(*p)++;
		}
		else if (c=='-'){
			c=**p;
			(*p)++;
			pw_s=-1;
		}
		int64_t pw=0;
		while (c>47&&c<58){
			pw=pw*10+(c-48);
			c=**p;
			(*p)++;
		}
		pw*=pw_s;
		v*=pow(2,(double)pw)*pow(5,(double)pw);
	}
	(*p)--;
	o->t=JSON_OBJECT_TYPE_FLOAT;
	o->dt.f=v*s;
	return 0;
}



json_object_t* get_by_key(json_object_t* json,const char* k){
	for (uint32_t i=0;i<json->dt.m.l;i++){
		json_map_keypair_t* e=json->dt.m.dt+i;
		for (uint32_t j=0;j<e->k.l+1;j++){
			if (*(e->k.v+j)!=*(k+j)){
				goto _check_next_key;
			}
		}
		return &(e->v);
_check_next_key:;
	}
	return NULL;
}



void free_json(json_object_t* json){
	if (json->t==JSON_OBJECT_TYPE_STRING){
		if (json->dt.s.v){
			free(json->dt.s.v);
		}
	}
	else if (json->t==JSON_OBJECT_TYPE_ARRAY){
		for (uint32_t i=0;i<json->dt.a.l;i++){
			free_json(json->dt.a.dt+i);
		}
		if (json->dt.a.dt){
			free(json->dt.a.dt);
		}
	}
	else if (json->t==JSON_OBJECT_TYPE_MAP){
		for (uint32_t i=0;i<json->dt.m.l;i++){
			json_map_keypair_t* e=json->dt.m.dt+i;
			if (e->k.v){
				free(e->k.v);
			}
			free_json(&(e->v));
		}
		if (json->dt.m.dt){
			free(json->dt.m.dt);
		}
	}
}



char* error_to_string(sll_error_t* e){
	char* o=malloc(512*sizeof(char));
	switch (e->t){
		default:
		case SLL_ERROR_UNKNOWN:
		case SLL_ERROR_NO_STACK:
		case SLL_ERROR_DIVISION_BY_ZERO:
		case SLL_ERROR_INVALID_FILE_FORMAT:
		case SLL_ERROR_STACK_CORRUPTED:
		case SLL_ERROR_INVALID_INSTRUCTION_INDEX:
		case SLL_ERROR_INVALID_STACK_INDEX:
			snprintf(o,512,"<type=%"PRIu8">",e->t);
			break;
		case SLL_ERROR_INTERNAL_STACK_OVERFLOW:
		case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
		case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
		case SLL_ERROR_UNMATCHED_OPEN_QUOTE:
		case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
		case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
		case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
		case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
		case SLL_ERROR_UNMATCHED_QUOTES:
		case SLL_ERROR_EMPTY_CHAR_STRING:
		case SLL_ERROR_UNTERMINATED_CHAR_STRING:
		case SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
		case SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER:
		case SLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
		case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
		case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
		case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
		case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
		case SLL_ERROR_UNKNOWN_SYMBOL:
		case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
		case SLL_ERROR_UNEXPECTED_CHARACTER:
		case SLL_ERROR_SYMBOL_TOO_LONG:
		case SLL_ERROR_NO_SYMBOL:
		case SLL_ERROR_TOO_MANY_ARGUMENTS:
		case SLL_ERROR_TOO_MANY_STATEMENTS:
		case SLL_ERROR_ARRAY_TOO_LONG:
		case SLL_ERROR_UNKNOWN_IDENTIFIER:
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
			snprintf(o,512,"<type=%"PRIu8", range=%"PRIu32"-%"PRIu32">",e->t,e->dt.r.off,e->dt.r.off+e->dt.r.sz);
			break;
		case SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION:
		case SLL_ERROR_SLL_ASSERTION:
			snprintf(o,512,"<type=%"PRIu8", string='%s'>",e->t,e->dt.str);
			break;
		case SLL_ERROR_INVALID_INSTRUCTION:
			snprintf(o,512,"<type=%"PRIu8", opcode=%"PRIu8">",e->t,e->dt.it);
			break;
	}
	return o;
}



void run_parser_test(const char* fp,test_result_t* o){
	FILE* f=fopen(fp,"rb");// lgtm [cpp/path-injection]
	char* f_dt=NULL;
	json_object_t json={
		JSON_OBJECT_TYPE_UNKNOWN
	};
	if (!f){
		goto _json_error;
	}
	fseek(f,0,SEEK_END);
	uint32_t sz=ftell(f);
	f_dt=malloc((sz+1)*sizeof(char));
	*(f_dt+sz)=0;
	fseek(f,0,SEEK_SET);
	if (fread(f_dt,sizeof(char),sz,f)!=sz){
		goto _json_error;
	}
	fclose(f);
	f=NULL;
	json_parser_state_t p=f_dt;
	if (parse_json(&p,&json)){
		goto _json_error;
	}
	free(f_dt);
	f_dt=NULL;
	json_object_t* nm_e=get_by_key(&json,"name");
	if (!nm_e||nm_e->t!=JSON_OBJECT_TYPE_STRING){
		goto _json_error;
	}
	char* nm=nm_e->dt.s.v;
	json_object_t* dt_e=get_by_key(&json,"data");
	if (!dt_e||dt_e->t!=JSON_OBJECT_TYPE_ARRAY){
		goto _json_error;
	}
	if (!dt_e->dt.a.l){
		printf("Empty Test '%s'\n",nm);
		return;
	}
	printf("Running Test '%s':\n",nm);
	char bf[32768];
	for (uint32_t i=0;i<dt_e->dt.a.l;i++){
		json_object_t* t=dt_e->dt.a.dt+i;
		if (t->t!=JSON_OBJECT_TYPE_MAP){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		json_object_t* in_e=get_by_key(t,"input");
		if (!in_e||in_e->t!=JSON_OBJECT_TYPE_STRING){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		uint32_t bfl;
		fflush(stdout);
		FILE* dt_f=fopen(t_fp,"wb");// lgtm [cpp/path-injection]
		if (!dt_f||fwrite(in_e->dt.s.v,sizeof(char),in_e->dt.s.l,dt_f)!=in_e->dt.s.l){
			if (dt_f){
				fclose(dt_f);
			}
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		fclose(dt_f);
		int8_t ec=execute_subprocess(TEST_ID_PARSE,bf,&bfl);
		if (ec==-1){
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		if (ec==1){
			o->f++;
			printf("-> Test Case #%"PRIu32": Program Crashed\n",i);
			continue;
		}
		FILE* c_f=fopen(c_fp,"rb");
		if (!c_f){
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		uint32_t j=0;
		for (;j<__coverage_map_length;j++){
			uint64_t v=0;
			if (fread(&v,sizeof(uint64_t),1,c_f)!=1){
				o->s++;
				printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
				break;
			}
			__coverage_map[j]|=v;
		}
		if (j!=__coverage_map_length){
			continue;
		}
		fclose(c_f);
		json_object_t* err_e=get_by_key(t,"error");
		if (!err_e||(err_e->t!=JSON_OBJECT_TYPE_NULL&&err_e->t!=JSON_OBJECT_TYPE_MAP)){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		if (err_e->t==JSON_OBJECT_TYPE_NULL){
			if (!bfl){
				o->p++;
				printf("-> Test Case #%"PRIu32": OK\n",i);
				continue;
			}
			if (bfl<sizeof(sll_error_t)){
				o->s++;
				printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
				continue;
			}
			o->f++;
			char* err_s=error_to_string((sll_error_t*)bf);
			printf("-> Test Case #%"PRIu32": Expected No Error, Recived %s\n",i,err_s);
			free(err_s);
			continue;
		}
		if (!bfl){
			o->f++;
			printf("-> Test Case #%"PRIu32": Expected an Error\n",i);
			continue;
		}
		if (bfl<sizeof(sll_error_t)){
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		sll_error_t* e=(sll_error_t*)bf;
		json_object_t* err_t_e=get_by_key(err_e,"type");
		if (!err_t_e||err_t_e->t!=JSON_OBJECT_TYPE_INTEGER||err_t_e->dt.i<0||err_t_e->dt.i>SLL_MAX_ERROR){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		sll_error_t ne={
			(sll_error_type_t)err_t_e->dt.i
		};
		json_object_t* err_v_e=get_by_key(err_e,"data");
		switch (ne.t){
			case SLL_ERROR_INTERNAL_STACK_OVERFLOW:
			case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			case SLL_ERROR_UNMATCHED_OPEN_QUOTE:
			case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_QUOTES:
			case SLL_ERROR_EMPTY_CHAR_STRING:
			case SLL_ERROR_UNTERMINATED_CHAR_STRING:
			case SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
			case SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER:
			case SLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
			case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			case SLL_ERROR_UNKNOWN_SYMBOL:
			case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			case SLL_ERROR_UNEXPECTED_CHARACTER:
			case SLL_ERROR_SYMBOL_TOO_LONG:
			case SLL_ERROR_NO_SYMBOL:
			case SLL_ERROR_TOO_MANY_ARGUMENTS:
			case SLL_ERROR_TOO_MANY_STATEMENTS:
			case SLL_ERROR_ARRAY_TOO_LONG:
			case SLL_ERROR_UNKNOWN_IDENTIFIER:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
				if (!err_v_e||err_v_e->t!=JSON_OBJECT_TYPE_ARRAY||err_v_e->dt.a.l!=2||err_v_e->dt.a.dt->t!=JSON_OBJECT_TYPE_INTEGER||err_v_e->dt.a.dt->dt.i<0||err_v_e->dt.a.dt->dt.i>UINT32_MAX||(err_v_e->dt.a.dt+1)->t!=JSON_OBJECT_TYPE_INTEGER||(err_v_e->dt.a.dt+1)->dt.i<0||(err_v_e->dt.a.dt+1)->dt.i>UINT32_MAX){
					o->s++;
					printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
					continue;
				}
				ne.dt.r.off=(sll_file_offset_t)err_v_e->dt.a.dt->dt.i;
				ne.dt.r.sz=(sll_file_offset_t)(err_v_e->dt.a.dt+1)->dt.i-ne.dt.r.off;
				break;
			case SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION:
			case SLL_ERROR_SLL_ASSERTION:
				if (!err_v_e||err_v_e->t!=JSON_OBJECT_TYPE_STRING||err_v_e->dt.s.l>255){
					o->s++;
					printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
					continue;
				}
				for (uint32_t j=0;j<err_v_e->dt.s.l;j++){
					ne.dt.str[j]=*(err_v_e->dt.s.v+j);
				}
				ne.dt.str[err_v_e->dt.s.l]=0;
				break;
			case SLL_ERROR_INVALID_INSTRUCTION:
				if (!err_v_e||err_v_e->t!=JSON_OBJECT_TYPE_INTEGER||err_v_e->dt.i<0||err_v_e->dt.i>UINT8_MAX){
					o->s++;
					printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
					continue;
				}
				ne.dt.it=(sll_assembly_instruction_type_t)err_v_e->dt.i;
				break;
		}
		if (e->t!=ne.t){
_wrong_error:
			o->f++;
			char* e_s=error_to_string(e);
			char* ne_s=error_to_string(&ne);
			printf("-> Test Case #%"PRIu32": Expected %s, Recived %s\n",i,ne_s,e_s);
			free(e_s);
			free(ne_s);
			continue;
		}
		switch (ne.t){
			case SLL_ERROR_INTERNAL_STACK_OVERFLOW:
			case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			case SLL_ERROR_UNMATCHED_OPEN_QUOTE:
			case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_QUOTES:
			case SLL_ERROR_EMPTY_CHAR_STRING:
			case SLL_ERROR_UNTERMINATED_CHAR_STRING:
			case SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
			case SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER:
			case SLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
			case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			case SLL_ERROR_UNKNOWN_SYMBOL:
			case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			case SLL_ERROR_UNEXPECTED_CHARACTER:
			case SLL_ERROR_SYMBOL_TOO_LONG:
			case SLL_ERROR_NO_SYMBOL:
			case SLL_ERROR_TOO_MANY_ARGUMENTS:
			case SLL_ERROR_TOO_MANY_STATEMENTS:
			case SLL_ERROR_ARRAY_TOO_LONG:
			case SLL_ERROR_UNKNOWN_IDENTIFIER:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
				if (ne.dt.r.off!=e->dt.r.off||ne.dt.r.sz!=e->dt.r.sz){
					goto _wrong_error;
				}
				break;
			case SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION:
			case SLL_ERROR_SLL_ASSERTION:
				for (uint16_t i=0;i<256;i++){
					if (ne.dt.str[i]!=e->dt.str[i]){
						goto _wrong_error;
					}
					if (!ne.dt.str[i]){
						break;
					}
				}
				break;
			case SLL_ERROR_INVALID_INSTRUCTION:
				if (ne.dt.it!=e->dt.it){
					goto _wrong_error;
				}
				break;
		}
		o->p++;
		printf("-> Test Case #%"PRIu32": OK\n",i);
	}
	free_json(&json);
	return;
_json_error:
	if (f){
		fclose(f);
	}
	if (f_dt){
		free(f_dt);
	}
	if (json.t!=JSON_OBJECT_TYPE_UNKNOWN){
		free_json(&json);
	}
	o->s++;
	printf("JSON Error in File '%s'. Skipping Test...\n",fp);
	return;
}



int main(int argc,const char** argv){
#ifdef _MSC_VER
	uint16_t i=(uint16_t)GetModuleFileNameA(NULL,e_fp,4096);
#else
	uint16_t i=(uint16_t)readlink("/proc/self/exe",e_fp,4096);
#endif
	e_fp[i]=0;
	while (i&&e_fp[i]!='\\'&&e_fp[i]!='/'){
		i--;
	}
	char c_o_fp[4096];
	for (uint16_t j=0;j<i;j++){
		t_fp[j]=e_fp[j];
		c_fp[j]=e_fp[j];
		c_o_fp[j]=e_fp[j];
	}
	t_fp[i]='/';
	t_fp[i+1]='t';
	t_fp[i+2]='m';
	t_fp[i+3]='p';
	t_fp[i+4]=0;
	c_fp[i]='/';
	c_fp[i+1]='c';
	c_fp[i+2]='o';
	c_fp[i+3]='v';
	c_fp[i+4]=0;
	memcpy(c_o_fp+i,"/coverage.txt",sizeof("/coverage.txt")/sizeof(char));
	if (argc>1){
		return execute_test(argv[1][0]-1);
	}
	char bf[4096]=__TEST_ROOT_DIR__;
	i=0;
	while (bf[i]){
		i++;
	}
	test_result_t dt={
		0,
		0,
		0
	};
	list_files(bf,i,run_parser_test,&dt);
	printf("%"PRIu32" Test%s Passed, %"PRIu32" Test%s Failed, %"PRIu32" Test%s Skipped\nWriting Coverage File '%s'...\n",dt.p,(dt.p==1?"":"s"),dt.f,(dt.f==1?"":"s"),dt.s,(dt.s==1?"":"s"),c_o_fp);
	FILE* c_o=fopen(c_o_fp,"wb");
	uint32_t c=0;
	for (uint32_t j=0;j<__coverage_map_length;j++){
		c+=(uint32_t)POPCNT(__coverage_map[j]);
	}
	fprintf(c_o,"Tested Code: (%.3f%%)\n",((float)c*100)/__coverage_count);
	for (uint32_t j=0;j<__coverage_map_length;j++){
		uint64_t v=__coverage_map[j];
		while (v){
			uint32_t k=FIND_FIRST_SET_BIT(v)|(j<<6);
			__coverage_data_t dt=__coverage_data[k];
			switch (dt.t){
				case __COVERAGE_DATA_TYPE_BREAK:
					fprintf(c_o,"  [%s:%u (%s)] 'break' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_CONTINUE:
					fprintf(c_o,"  [%s:%u (%s)] 'continue' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_FUNC:
					fprintf(c_o,"  [%s:%u (%s)] Function Entry\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_FUNC_END:
					fprintf(c_o,"  [%s:%u (%s)] Implict Return\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_GOTO:
					fprintf(c_o,"  [%s:%u (%s)] 'goto' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_RETURN:
					fprintf(c_o,"  [%s:%u (%s)] 'return' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
			}
			v&=v-1;
		}
	}
	fprintf(c_o,"\nUntested Code: (%.3f%%)\n",((float)(__coverage_count-c)*100)/__coverage_count);
	for (uint32_t j=0;j<__coverage_map_length;j++){
		uint64_t v=~__coverage_map[j];
		while (v){
			uint32_t k=FIND_FIRST_SET_BIT(v)|(j<<6);
			if (k==__coverage_count){
				break;
			}
			__coverage_data_t dt=__coverage_data[k];
			switch (dt.t){
				case __COVERAGE_DATA_TYPE_BREAK:
					fprintf(c_o,"  [%s:%u (%s)] 'break' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_CONTINUE:
					fprintf(c_o,"  [%s:%u (%s)] 'continue' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_FUNC:
					fprintf(c_o,"  [%s:%u (%s)] Function '%s'\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm],__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_FUNC_END:
					fprintf(c_o,"  [%s:%u (%s)] Implict Return\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_GOTO:
					fprintf(c_o,"  [%s:%u (%s)] 'goto' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
				case __COVERAGE_DATA_TYPE_RETURN:
					fprintf(c_o,"  [%s:%u (%s)] 'return' statement\n",__coverage_strings[dt.fp],dt.ln,__coverage_strings[dt.nm]);
					break;
			}
			v&=v-1;
		}
	}
	fclose(c_o);
	printf("Test Coverage: %.3f%%\n",((float)c*100)/__coverage_count);
	return !!dt.f;
}

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#else
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#endif
#include <run_tests.h>
#include <sll_standalone.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char e_fp[4096];
char ti_fp[4096];
char to_fp[4096];



uint8_t execute_test(uint8_t id){
	remove(to_fp);
	if (id==TEST_ID_PARSE){
		sll_internal_function_table_t i_ft;
		sll_create_internal_function_table(&i_ft);
		sll_register_standard_internal_functions(&i_ft);
		FILE* f=fopen(ti_fp,"rb");
		if (!f){
			return 1;
		}
		sll_input_data_stream_t is;
		sll_stream_create_input_from_file(f,&is);
		sll_compilation_data_t c_dt;
		sll_init_compilation_data(SLL_CHAR("<internal>"),&is,&c_dt);
		sll_error_t e;
		if (!sll_parse_all_objects(&c_dt,&i_ft,NULL,&e)){
			FILE* o_f=fopen(to_fp,"wb");
			if (!o_f){
				return 1;
			}
			fwrite((void*)&e,sizeof(sll_error_t),1,o_f);
			fclose(o_f);
		}
		sll_deinit_compilation_data(&c_dt);
		fclose(f);
		return 0;
	}
	return 1;
}



int8_t execute_subprocess(uint8_t id){
#ifdef _MSC_VER
	uint16_t i=0;
	while (e_fp[i]){
		i++;
	}
	e_fp[i]=' ';
	e_fp[i+1]=id+1;
	e_fp[i+2]=0;
	STARTUPINFOA si={0};
	si.cb=sizeof(STARTUPINFOA);
	si.dwFlags=0;
	PROCESS_INFORMATION pi;
	if (!CreateProcessA(NULL,e_fp,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi)){
		return -1;
	}
	e_fp[i]=0;
	DWORD ec;
	if (WaitForSingleObject(pi.hProcess,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(pi.hProcess,&ec)){
		return -1;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return !!ec;
#else
	pid_t f=fork();
	if (f==-1){
		return -1;
	}
	if (f==0){
		exit(execute_test(id));
	}
	int st;
	if (waitpid(-1,&st,0)==-1){
		return -1;
	}
	return (!WIFEXITED(st)||WEXITSTATUS(st));
#endif
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
		case SLL_ERROR_UNRELEASED_OBJECTS:
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
		case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
		case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
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
		case SLL_ERROR_ARRAY_TOO_LONG:
		case SLL_ERROR_MAP_TOO_LONG:
		case SLL_ERROR_UNKNOWN_IDENTIFIER:
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
		case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
			snprintf(o,512,"<type=%"PRIu8", range=%"PRIu32"-%"PRIu32">",e->t,e->dt.r.off,e->dt.r.off+e->dt.r.sz);
			break;
		case SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION:
		case SLL_ERROR_ASSERTION:
			snprintf(o,512,"<type=%"PRIu8", string='%s'>",e->t,e->dt.str);
			break;
		case SLL_ERROR_INVALID_INSTRUCTION:
			snprintf(o,512,"<type=%"PRIu8", opcode=%"PRIu8">",e->t,e->dt.it);
			break;
	}
	return o;
}



void run_parser_test(const char* fp,test_result_t* o){
	FILE* f=fopen(fp,"rb");
	char* f_dt=NULL;
	sll_json_object_t json={
		SLL_JSON_OBJECT_TYPE_INTEGER,
		{
			.i=0
		}
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
	sll_json_parser_state_t p=(sll_json_parser_state_t)f_dt;
	if (!sll_json_parse(&p,&json)){
		goto _json_error;
	}
	free(f_dt);
	f_dt=NULL;
	sll_string_t tmp_str;
	sll_string_from_pointer(SLL_CHAR("name"),&tmp_str);
	sll_json_object_t* nm_e=sll_json_get_by_key(&json,&tmp_str);
	if (!nm_e||nm_e->t!=SLL_JSON_OBJECT_TYPE_STRING){
		goto _json_error;
	}
	char* nm=(char*)nm_e->dt.s.v;
	sll_string_from_pointer(SLL_CHAR("data"),&tmp_str);
	sll_json_object_t* dt_e=sll_json_get_by_key(&json,&tmp_str);
	if (!dt_e||dt_e->t!=SLL_JSON_OBJECT_TYPE_ARRAY){
		goto _json_error;
	}
	if (!dt_e->dt.a.l){
		printf("Empty Test '%s'\n",nm);
		return;
	}
	printf("Running Test '%s':\n",nm);
	uint8_t bf[32768];
	for (uint32_t i=0;i<dt_e->dt.a.l;i++){
		sll_json_object_t* t=dt_e->dt.a.dt+i;
		if (t->t!=SLL_JSON_OBJECT_TYPE_MAP){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		sll_string_from_pointer(SLL_CHAR("input"),&tmp_str);
		sll_json_object_t* in_e=sll_json_get_by_key(t,&tmp_str);
		if (!in_e||in_e->t!=SLL_JSON_OBJECT_TYPE_STRING){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		FILE* dt_f=fopen(ti_fp,"wb");
		if (!dt_f||fwrite(in_e->dt.s.v,sizeof(char),in_e->dt.s.l,dt_f)!=in_e->dt.s.l){
			if (dt_f){
				fclose(dt_f);
			}
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		fclose(dt_f);
		fflush(stdout);
		int8_t ec=execute_subprocess(TEST_ID_PARSE);
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
		sll_string_from_pointer(SLL_CHAR("error"),&tmp_str);
		sll_json_object_t* err_e=sll_json_get_by_key(t,&tmp_str);
		if (!err_e||(err_e->t!=SLL_JSON_OBJECT_TYPE_NULL&&err_e->t!=SLL_JSON_OBJECT_TYPE_MAP)){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		uint32_t bfl=0;
		FILE* bf_f=fopen(to_fp,"rb");
		if (bf_f){
			fseek(bf_f,0,SEEK_END);
			bfl=ftell(bf_f);
			fseek(bf_f,0,SEEK_SET);
			if (fread(bf,sizeof(uint8_t),bfl,bf_f)!=bfl){
				fclose(bf_f);
				continue;
			}
			fclose(bf_f);
		}
		if (err_e->t==SLL_JSON_OBJECT_TYPE_NULL){
			if (!bfl){
				o->p++;
				printf("-> Test Case #%"PRIu32": OK\n",i);
				continue;
			}
			if (bfl!=sizeof(sll_error_t)){
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
		if (bfl!=sizeof(sll_error_t)){
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		sll_error_t* e=(sll_error_t*)bf;
		sll_string_from_pointer(SLL_CHAR("type"),&tmp_str);
		sll_json_object_t* err_t_e=sll_json_get_by_key(err_e,&tmp_str);
		if (!err_t_e||err_t_e->t!=SLL_JSON_OBJECT_TYPE_INTEGER||err_t_e->dt.i<0||err_t_e->dt.i>SLL_MAX_ERROR){
			o->s++;
			printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
			continue;
		}
		sll_error_t ne={
			(sll_error_type_t)err_t_e->dt.i
		};
		sll_string_from_pointer(SLL_CHAR("data"),&tmp_str);
		sll_json_object_t* err_v_e=sll_json_get_by_key(err_e,&tmp_str);
		switch (ne.t){
			case SLL_ERROR_INTERNAL_STACK_OVERFLOW:
			case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			case SLL_ERROR_UNMATCHED_OPEN_QUOTE:
			case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
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
			case SLL_ERROR_ARRAY_TOO_LONG:
			case SLL_ERROR_MAP_TOO_LONG:
			case SLL_ERROR_UNKNOWN_IDENTIFIER:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
				if (!err_v_e||err_v_e->t!=SLL_JSON_OBJECT_TYPE_ARRAY||err_v_e->dt.a.l!=2||err_v_e->dt.a.dt->t!=SLL_JSON_OBJECT_TYPE_INTEGER||err_v_e->dt.a.dt->dt.i<0||err_v_e->dt.a.dt->dt.i>SLL_MAX_FILE_OFFSET||(err_v_e->dt.a.dt+1)->t!=SLL_JSON_OBJECT_TYPE_INTEGER||(err_v_e->dt.a.dt+1)->dt.i<0||(err_v_e->dt.a.dt+1)->dt.i>UINT32_MAX){
					o->s++;
					printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
					continue;
				}
				ne.dt.r.off=(sll_file_offset_t)err_v_e->dt.a.dt->dt.i;
				ne.dt.r.sz=(sll_file_offset_t)(err_v_e->dt.a.dt+1)->dt.i-ne.dt.r.off;
				break;
			case SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION:
			case SLL_ERROR_ASSERTION:
				if (!err_v_e||err_v_e->t!=SLL_JSON_OBJECT_TYPE_STRING||err_v_e->dt.s.l>255){
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
				if (!err_v_e||err_v_e->t!=SLL_JSON_OBJECT_TYPE_INTEGER||err_v_e->dt.i<0||err_v_e->dt.i>SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE){
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
			case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
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
			case SLL_ERROR_ARRAY_TOO_LONG:
			case SLL_ERROR_MAP_TOO_LONG:
			case SLL_ERROR_UNKNOWN_IDENTIFIER:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG:
			case SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII:
				if (ne.dt.r.off!=e->dt.r.off||ne.dt.r.sz!=e->dt.r.sz){
					goto _wrong_error;
				}
				break;
			case SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION:
			case SLL_ERROR_ASSERTION:
				if (strcmp((char*)ne.dt.str,(char*)e->dt.str)){
					goto _wrong_error;
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
	sll_deinit_json_object(&json);
	return;
_json_error:
	if (f){
		fclose(f);
	}
	if (f_dt){
		free(f_dt);
	}
	sll_deinit_json_object(&json);
	o->s++;
	printf("JSON Error in File '%s'. Skipping Test...\n",fp);
	return;
}



int main(int argc,const char** argv){
	sll_init();
#ifdef _MSC_VER
	uint16_t i=(uint16_t)GetModuleFileNameA(NULL,e_fp,4096);
#else
	uint16_t i=(uint16_t)readlink("/proc/self/exe",e_fp,4096);
#endif
	e_fp[i]=0;
	while (i&&e_fp[i]!='\\'&&e_fp[i]!='/'){
		i--;
	}
	sll_char_t td_fp[4096];
	for (uint16_t j=0;j<i;j++){
		ti_fp[j]=e_fp[j];
		to_fp[j]=e_fp[j];
		td_fp[j]=e_fp[j];
	}
	memcpy(ti_fp+i,"/tmp_in",8);
	memcpy(to_fp+i,"/tmp_out",9);
	memcpy(td_fp+i,"/../tests/data/",16);
#ifdef _MSC_VER
	if (argc==2){
		return execute_test(argv[1][0]-1);
	}
#endif
	test_result_t dt={
		0,
		0,
		0
	};
	sll_string_t* a=NULL;
	sll_array_length_t l=sll_platform_list_directory_recursive(td_fp,&a);
	for (sll_array_length_t j=0;j<l;j++){
		run_parser_test((char*)((a+j)->v),&dt);
		sll_deinit_string(a+j);
	}
	sll_deallocate(a);
	printf("%"PRIu32" Test%s Passed, %"PRIu32" Test%s Failed, %"PRIu32" Test%s Skipped\n",dt.p,(dt.p==1?"":"s"),dt.f,(dt.f==1?"":"s"),dt.s,(dt.s==1?"":"s"));
	return !!dt.f;
}

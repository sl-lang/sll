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



sll_char_t e_fp[SLL_API_MAX_FILE_PATH_LENGTH];
sll_char_t ti_fp[SLL_API_MAX_FILE_PATH_LENGTH];
sll_char_t to_fp[SLL_API_MAX_FILE_PATH_LENGTH];



static sll_bool_t execute_test(uint8_t id){
	remove((char*)to_fp);
	if (id==TEST_ID_PARSE){
		sll_internal_function_table_t i_ft;
		sll_create_internal_function_table(&i_ft);
		sll_register_builtin_internal_functions(&i_ft);
		sll_file_t f;
		if (!sll_file_open(ti_fp,SLL_FILE_FLAG_READ,&f)){
			return 0;
		}
		sll_compilation_data_t c_dt;
		sll_init_compilation_data(SLL_CHAR("<internal>"),&f,&c_dt);
		sll_error_t e;
		if (!sll_parse_all_nodes(&c_dt,&i_ft,NULL,&e)){
			sll_file_t of;
			if (!sll_file_open(to_fp,SLL_FILE_FLAG_WRITE,&of)){
				return 1;
			}
			sll_file_write(&of,&e,sizeof(sll_error_t));
			sll_file_close(&of);
		}
		sll_free_compilation_data(&c_dt);
		sll_file_close(&f);
		return 1;
	}
	return 0;
}



static int8_t execute_subprocess(uint8_t id){
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
		exit(!execute_test(id));
	}
	int st;
	if (waitpid(-1,&st,0)==-1){
		return -1;
	}
	return (!WIFEXITED(st)||WEXITSTATUS(st));
#endif
}



static sll_char_t* error_to_string(sll_error_t* e){
	sll_char_t* o=malloc(512*sizeof(sll_char_t));
	switch (e->t){
		default:
		case SLL_ERROR_UNKNOWN:
		case SLL_ERROR_INVALID_FILE_FORMAT:
		case SLL_ERROR_INVALID_INSTRUCTION_INDEX:
		case SLL_ERROR_INVALID_STACK_INDEX:
			snprintf((char*)o,512,"<type=%"PRIu8">",e->t);
			break;
		case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
		case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
		case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
		case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
		case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
		case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
		case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
		case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
		case SLL_ERROR_UNMATCHED_QUOTES:
		case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
		case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
		case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
		case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
		case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
		case SLL_ERROR_UNEXPECTED_CHARACTER:
		case SLL_ERROR_UNKNOWN_IDENTIFIER:
			snprintf((char*)o,512,"<type=%"PRIu8", range=%"PRIu64"-%"PRIu64">",e->t,e->dt.r.off,e->dt.r.off+e->dt.r.sz);
			break;
		case SLL_ERROR_INVALID_INSTRUCTION:
			snprintf((char*)o,512,"<type=%"PRIu8", opcode=%"PRIu8">",e->t,e->dt.it);
			break;
	}
	return o;
}



static void run_parser_test(const sll_char_t* fp,test_result_t* o){
	FILE* f=fopen((char*)fp,"rb");
	sll_char_t* f_dt=NULL;
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
	sll_string_length_t sz=ftell(f);
	f_dt=malloc((sz+1)*sizeof(sll_char_t));
	*(f_dt+sz)=0;
	fseek(f,0,SEEK_SET);
	if (fread(f_dt,sizeof(sll_char_t),sz,f)!=sz){
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
	sll_char_t* nm=(sll_char_t*)nm_e->dt.s.v;
	sll_string_from_pointer(SLL_CHAR("data"),&tmp_str);
	sll_json_object_t* dt_e=sll_json_get_by_key(&json,&tmp_str);
	if (!dt_e||dt_e->t!=SLL_JSON_OBJECT_TYPE_ARRAY){
		goto _json_error;
	}
	if (!dt_e->dt.a.l){
		printf("Empty Test '%s'\n",(char*)nm);
		return;
	}
	printf("Running Test '%s':\n",(char*)nm);
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
		sll_file_t dt_f;
		if (!sll_file_open(ti_fp,SLL_FILE_FLAG_WRITE,&dt_f)){
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		if (sll_file_write(&dt_f,in_e->dt.s.v,in_e->dt.s.l)!=in_e->dt.s.l){
			sll_file_close(&dt_f);
			o->s++;
			printf("-> Internal Error in Test Case #%"PRIu32" (Line %u)\n",i,__LINE__);
			continue;
		}
		sll_file_close(&dt_f);
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
		FILE* bf_f=fopen((char*)to_fp,"rb");
		sll_string_length_t bfl=0;
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
			sll_char_t* err_s=error_to_string((sll_error_t*)bf);
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
			case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_QUOTES:
			case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			case SLL_ERROR_UNEXPECTED_CHARACTER:
			case SLL_ERROR_UNKNOWN_IDENTIFIER:
				if (!err_v_e||err_v_e->t!=SLL_JSON_OBJECT_TYPE_ARRAY||err_v_e->dt.a.l!=2||err_v_e->dt.a.dt->t!=SLL_JSON_OBJECT_TYPE_INTEGER||err_v_e->dt.a.dt->dt.i<0||err_v_e->dt.a.dt->dt.i>SLL_MAX_FILE_OFFSET||(err_v_e->dt.a.dt+1)->t!=SLL_JSON_OBJECT_TYPE_INTEGER||(err_v_e->dt.a.dt+1)->dt.i<0||(err_v_e->dt.a.dt+1)->dt.i>UINT32_MAX){
					o->s++;
					printf("-> JSON Error in Test Case #%"PRIu32"\n",i);
					continue;
				}
				ne.dt.r.off=(sll_file_offset_t)err_v_e->dt.a.dt->dt.i;
				ne.dt.r.sz=(sll_file_offset_t)(err_v_e->dt.a.dt+1)->dt.i-ne.dt.r.off;
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
			sll_char_t* e_s=error_to_string(e);
			sll_char_t* ne_s=error_to_string(&ne);
			printf("-> Test Case #%"PRIu32": Expected %s, Recived %s\n",i,ne_s,e_s);
			free(e_s);
			free(ne_s);
			continue;
		}
		switch (ne.t){
			case SLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			case SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			case SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS:
			case SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS:
			case SLL_ERROR_UNMATCHED_QUOTES:
			case SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			case SLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			case SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			case SLL_ERROR_UNEXPECTED_CHARACTER:
			case SLL_ERROR_UNKNOWN_IDENTIFIER:
				if (ne.dt.r.off!=e->dt.r.off||ne.dt.r.sz!=e->dt.r.sz){
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
	sll_free_json_object(&json);
	return;
_json_error:
	if (f){
		fclose(f);
	}
	if (f_dt){
		free(f_dt);
	}
	sll_free_json_object(&json);
	o->s++;
	printf("JSON Error in File '%s'. Skipping Test...\n",fp);
	return;
}



int main(int argc,const char** argv){
	sll_init();
	sll_string_length_t i=sll_platform_get_executable_file_path(e_fp,SLL_API_MAX_FILE_PATH_LENGTH);
	while (i&&e_fp[i]!='\\'&&e_fp[i]!='/'){
		i--;
	}
	sll_char_t td_fp[SLL_API_MAX_FILE_PATH_LENGTH];
	for (sll_string_length_t j=0;j<i;j++){
		ti_fp[j]=e_fp[j];
		to_fp[j]=e_fp[j];
		td_fp[j]=e_fp[j];
	}
	SLL_COPY_STRING_NULL(SLL_CHAR("/tmp_in"),ti_fp+i);
	SLL_COPY_STRING_NULL(SLL_CHAR("/tmp_out"),to_fp+i);
#ifdef _MSC_VER
	if (argc==2){
		return !execute_test(argv[1][0]-1);
	}
#endif
	SLL_COPY_STRING_NULL(SLL_CHAR("/../tests/data/"),td_fp+i);
	test_result_t dt={
		0,
		0,
		0
	};
	sll_string_t* a=NULL;
	sll_array_length_t l=sll_platform_list_directory_recursive(td_fp,&a);
	for (sll_array_length_t j=0;j<l;j++){
		run_parser_test((a+j)->v,&dt);
		sll_free_string(a+j);
	}
	sll_deallocate(a);
	printf("%"PRIu32" Test%s Passed, %"PRIu32" Test%s Failed, %"PRIu32" Test%s Skipped\n",dt.p,(dt.p==1?"":"s"),dt.f,(dt.f==1?"":"s"),dt.s,(dt.s==1?"":"s"));
	return !!dt.f;
}

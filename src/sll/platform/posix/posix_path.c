#include <sll/_sll_internal.h>
#include <sll/api/path.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/env.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>



static void _list_dir_files(sll_char_t* bf,sll_string_length_t i,file_list_data_t* o){
	bf[i]=0;
	DIR* d=opendir((char*)bf);
	if (d){
		struct dirent* dt;
		while ((dt=readdir(d))){
			if (dt->d_type==DT_REG){
				sll_string_length_t j=sll_string_length_unaligned(SLL_CHAR(dt->d_name));
				o->l++;
				o->dt=sll_reallocate(o->dt,o->l*sizeof(sll_string_t));
				sll_string_t* s=o->dt+o->l-1;
				sll_string_create(i+j,s);
				sll_string_insert_pointer_length(bf,i,0,s);
				sll_string_insert_pointer_length(SLL_CHAR(dt->d_name),j,i,s);
			}
			else if (dt->d_type==DT_DIR){
				if (*(dt->d_name)=='.'&&(*(dt->d_name+1)==0||(*(dt->d_name+1)=='.'&&*(dt->d_name+2)==0))){
					continue;
				}
				sll_string_length_t j=sll_string_length_unaligned(SLL_CHAR(dt->d_name));
				sll_copy_data(dt->d_name,j,bf+i);
				bf[i+j]='/';
				_list_dir_files(bf,i+j+1,o);
			}
		}
		closedir(d);
	}
}



__SLL_EXTERNAL sll_string_length_t sll_platform_absolute_path(const sll_char_t* lnk,sll_char_t* o,sll_string_length_t ol){
	if (ol>=PATH_MAX){
		if (!realpath((const char*)lnk,(char*)o)){
			return 0;
		}
		return sll_string_length_unaligned(o);
	}
	sll_char_t bf[PATH_MAX];
	if (!realpath((const char*)lnk,(char*)bf)){
		return 0;
	}
	sll_string_length_t l=sll_string_length_unaligned(bf)+1;
	if (l>ol){
		l=ol;
	}
	sll_copy_data(bf,l,o);
	return l;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol){
	if (!getcwd((char*)o,ol)){
		*o=0;
		return 0;
	}
	return sll_string_length_unaligned(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_executable_file_path(sll_char_t* o,sll_string_length_t ol){
	ssize_t i=readlink("/proc/self/exe",(char*)o,ol-1);
	if (i!=-1){
		*(o+i)=0;
		return (sll_string_length_t)i;
	}
	*o=0;
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_library_file_path(sll_char_t* o,sll_string_length_t ol){
	Dl_info dt;
	if (!dladdr(sll_platform_get_library_file_path,&dt)){
		return 0;
	}
	return sll_platform_absolute_path((const sll_char_t*)(dt.dli_fname),o,ol);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_temporary_file_path(sll_char_t* o,sll_string_length_t ol){
	sll_string_t k;
	sll_string_from_pointer(SLL_CHAR("TMPDIR"),&k);
	sll_string_t v;
	sll_bool_t st=sll_get_environment_variable(&k,&v);
	sll_free_string(&k);
	if (st){
		sll_string_length_t l=(v.l>ol?ol:v.l);
		sll_copy_data(v.v,l,o);
		sll_free_string(&v);
		return l;
	}
	sll_string_length_t l=(ol<4?ol:4);
	sll_copy_data("/tmp",l,o);
	return l;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o){
	DIR* d=opendir((char*)fp);
	sll_string_t* op=sll_allocate_stack(1);
	sll_array_length_t ol=0;
	if (d){
		struct dirent* dt;
		while ((dt=readdir(d))){
			if (dt->d_type==DT_DIR&&*(dt->d_name)=='.'&&(*(dt->d_name+1)==0||(*(dt->d_name+1)=='.'&&*(dt->d_name+2)==0))){
				continue;
			}
			if (!(ol&7)){
				void* tmp=sll_reallocate(op,(ol+8)*sizeof(sll_string_t));
				if (!tmp){
					*o=sll_reallocate(op,ol*sizeof(sll_string_t));
					closedir(d);
					return ol-1;
				}
				op=tmp;
			}
			sll_string_length_t l=sll_string_length_unaligned(SLL_CHAR(dt->d_name));
			sll_string_from_pointer_length(SLL_CHAR(dt->d_name),l,op+ol);
			ol++;
		}
		closedir(d);
	}
	*o=sll_reallocate(op,ol*sizeof(sll_string_t));
	return ol;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o){
	sll_char_t bf[PATH_MAX+1];
	sll_string_length_t l=sll_string_length_unaligned(fp);
	sll_copy_data(fp,l,bf);
	if (bf[l-1]!='/'&&bf[l-1]!='\\'){
		bf[l]='/';
		l++;
	}
	file_list_data_t dt={
		sll_allocate_stack(1),
		0
	};
	_list_dir_files(bf,l,&dt);
	if (!dt.l){
		sll_deallocate(dt.dt);
		dt.dt=NULL;
	}
	*o=dt.dt;
	return dt.l;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp){
	return !access((char*)fp,F_OK);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp){
	struct stat st;
	return (!stat((char*)fp,&st)&&(st.st_mode&S_IFDIR));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_current_working_directory(const sll_char_t* p){
	return !chdir((char*)p);
}

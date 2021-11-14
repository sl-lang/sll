#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/init.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_FUNC void sll_file_close(sll_file_t* f){
	sll_file_flush(f);
	sll_platform_file_close(f->fd);
	sll_deinit_string((sll_string_t*)(&(f->nm)));
}



__SLL_FUNC void sll_file_flush(sll_file_t* f){
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_file_open(const sll_char_t* nm,sll_file_flags_t f,sll_file_t* o){
	sll_file_descriptor_t fd=sll_platform_file_open(nm,f);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return 0;
	}
	*((sll_file_descriptor_t*)(&(o->fd)))=fd;
	sll_string_from_pointer(nm,(sll_string_t*)(&(o->nm)));
	*((sll_file_flags_t*)(&(o->f)))=f;
	return 1;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* p,sll_size_t sz){
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return 0;
	}
	return sll_platform_file_read(f->fd,p,sz);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_size_t sll_file_write(sll_file_t* f,const void* p,sll_size_t sz){
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	return sll_platform_file_write(f->fd,p,sz);
}

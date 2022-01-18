#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#ifdef DEBUG_BUILD
#define EXTRA_FLAGS SLL_FILE_FLAG_NO_BUFFER
#else
#define EXTRA_FLAGS 0
#endif



static sll_file_t _file_stdin;
static sll_file_t _file_stdout;
static sll_file_t _file_stderr;



__SLL_EXTERNAL sll_file_t* sll_stdin=&_file_stdin;
__SLL_EXTERNAL sll_file_t* sll_stdout=&_file_stdout;
__SLL_EXTERNAL sll_file_t* sll_stderr=&_file_stderr;



void _file_init_std_streams(void){
	IGNORE_RESULT(sll_file_open_descriptor(SLL_CHAR("sll_stdin"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_INPUT),SLL_FILE_FLAG_READ|SLL_FILE_FLAG_NO_BUFFER|FILE_FLAG_NO_RELEASE,sll_stdin));
	IGNORE_RESULT(sll_file_open_descriptor(SLL_CHAR("sll_stdout"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_OUTPUT),SLL_FILE_FLAG_WRITE|FILE_FLAG_NO_RELEASE|EXTRA_FLAGS,sll_stdout));
	IGNORE_RESULT(sll_file_open_descriptor(SLL_CHAR("sll_stderr"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR),SLL_FILE_FLAG_WRITE|FILE_FLAG_NO_RELEASE|EXTRA_FLAGS,sll_stderr));
}



void _file_release_std_streams(void){
	(*((sll_file_flags_t*)(&(sll_stdin->f))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stdout->f))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stderr->f))))&=~FILE_FLAG_NO_RELEASE;
	sll_file_close(sll_stdin);
	sll_file_close(sll_stdout);
	sll_file_close(sll_stderr);
}



__SLL_EXTERNAL void sll_file_close(sll_file_t* f){
	sll_file_flush(f);
	if (f->f&FILE_FLAG_NO_RELEASE){
		return;
	}
	if (!(f->f&FILE_FLAG_MEMORY)){
		sll_platform_file_close(f->dt.fl.fd);
		sll_free_string((sll_string_t*)(&(f->dt.fl.nm)));
		if (!(f->f&SLL_FILE_FLAG_NO_BUFFER)){
			if (f->f&SLL_FILE_FLAG_READ){
				sll_platform_free_page(f->_r_bf,SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE));
			}
			if (f->f&SLL_FILE_FLAG_WRITE){
				sll_platform_free_page(f->_w_bf,SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE));
			}
		}
	}
	else{
		SLL_ASSERT(f->f&SLL_FILE_FLAG_NO_BUFFER);
		sll_deallocate((void*)(f->dt.mm.p));
	}
	*((sll_file_flags_t*)(&(f->f)))=FILE_FLAG_NO_RELEASE;
}



__SLL_EXTERNAL void sll_file_flush(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_WRITE)||(f->f&SLL_FILE_FLAG_NO_BUFFER)){
		return;
	}
	sll_platform_file_write(f->dt.fl.fd,f->_w_bf,f->_w_bf_off);
	f->_w_bf_off=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_from_data(void* p,sll_size_t sz,sll_file_flags_t f,sll_file_t* o){
	void* n=sll_allocate(sz);
	if (!n){
		return 0;
	}
	sll_copy_data(p,sz,n);
	*((void**)(&(o->dt.mm.p)))=n;
	*((sll_size_t*)(&(o->dt.mm.sz)))=sz;
	*((sll_file_flags_t*)(&(o->f)))=f|SLL_FILE_FLAG_NO_BUFFER|FILE_FLAG_MEMORY;
	o->_l_num=0;
	o->_l_off=0;
	o->_off=0;
	if (f&SLL_FILE_FLAG_WRITE){
		SLL_UNIMPLEMENTED();
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_open(const sll_char_t* nm,sll_file_flags_t f,sll_file_t* o){
	SLL_ASSERT(!(f&FILE_FLAG_MEMORY));
	if ((f&SLL_FILE_FLAG_READ)&&!sll_platform_path_exists(nm)){
		return 0;
	}
	return sll_file_open_descriptor(nm,sll_platform_file_open(nm,f),f,o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_open_descriptor(const sll_char_t* nm,sll_file_descriptor_t fd,sll_file_flags_t f,sll_file_t* o){
	SLL_ASSERT(!(f&FILE_FLAG_MEMORY));
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return 0;
	}
	*((sll_file_descriptor_t*)(&(o->dt.fl.fd)))=fd;
	sll_string_from_pointer(nm,(sll_string_t*)(&(o->dt.fl.nm)));
	*((sll_file_flags_t*)(&(o->f)))=f;
	o->_l_num=0;
	o->_l_off=0;
	o->_off=0;
	if (!(f&SLL_FILE_FLAG_NO_BUFFER)){
		if (f&SLL_FILE_FLAG_READ){
			o->_r_bf=sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE),1);
			o->_r_bf_off=0;
			o->_r_bf_sz=0;
		}
		if (f&SLL_FILE_FLAG_WRITE){
			o->_w_bf=sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE),1);
			o->_w_bf_off=0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_peek_char(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off==f->dt.mm.sz){
			return SLL_END_OF_DATA;
		}
		return *((sll_char_t*)((void*)((uint64_t)(f->dt.mm.p)+f->_off)));
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* p,sll_size_t sz){
	if (!(f->f&SLL_FILE_FLAG_READ)||!sz){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off+sz>=f->dt.mm.sz){
			sz=f->dt.mm.sz-f->_off;
		}
		sll_copy_data((void*)((uint64_t)(f->dt.mm.p)+f->_off),sz,p);
		f->_off+=sz;
		return sz;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sz=sll_platform_file_read(f->dt.fl.fd,p,sz);
		f->_off+=sz;
		return sz;
	}
	if (!f->_r_bf_off&&!f->_r_bf_sz){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE);
		if (!f->_r_bf_sz){
			return 0;
		}
	}
	if (f->_r_bf_off+sz<=f->_r_bf_sz){
		sll_copy_data(f->_r_bf+f->_r_bf_off,sz,p);
		f->_r_bf_off+=sz;
		if (f->_r_bf_off==f->_r_bf_sz){
			f->_r_bf_off=0;
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE);
		}
		return sz;
	}
	sll_size_t o=f->_r_bf_sz-f->_r_bf_off;
	sz-=o;
	sll_copy_data(f->_r_bf+f->_r_bf_off,o,p);
	p=(void*)(((uint64_t)p)+o);
	while (sz>FILE_BUFFER_SIZE){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,p,FILE_BUFFER_SIZE);
		if (f->_r_bf_sz!=FILE_BUFFER_SIZE){
			SLL_UNIMPLEMENTED();
			f->_r_bf_off=0;
			f->_r_bf_sz=0;
			return o;
		}
		o+=FILE_BUFFER_SIZE;
		sz-=FILE_BUFFER_SIZE;
		p=(void*)(((uint64_t)p)+FILE_BUFFER_SIZE);
	}
	f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE);
	if (f->_r_bf_sz<sz){
		o+=f->_r_bf_sz;
		sll_copy_data(f->_r_bf,f->_r_bf_sz,p);
		f->_r_bf_off=0;
		f->_r_bf_sz=0;
		return o;
	}
	o+=sz;
	f->_r_bf_off=sz;
	sll_copy_data(f->_r_bf,sz,p);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_read_char(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off==f->dt.mm.sz){
			return SLL_END_OF_DATA;
		}
		sll_char_t o=*((sll_char_t*)((void*)((uint64_t)(f->dt.mm.p)+f->_off)));
		f->_off++;
		return o;
	}
	sll_char_t o;
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		if (!sll_platform_file_read(f->dt.fl.fd,&o,sizeof(sll_char_t))){
			return SLL_END_OF_DATA;
		}
	}
	else{
		if (!f->_r_bf_off&&!f->_r_bf_sz){
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE);
			if (!f->_r_bf_sz){
				return SLL_END_OF_DATA;
			}
		}
		o=f->_r_bf[f->_r_bf_off];
		f->_r_bf_off++;
		if (f->_r_bf_off==f->_r_bf_sz){
			f->_r_bf_off=0;
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE);
		}
	}
	f->_off++;
	if (o=='\n'){
		f->_l_num++;
		f->_l_off=f->_off;
	}
	return o;
}



__SLL_EXTERNAL void sll_file_reset(sll_file_t* f){
	if (!(f->f&FILE_FLAG_MEMORY)){
		sll_platform_file_seek(f->dt.fl.fd,0);
		f->_r_bf_off=0;
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE);
	}
	f->_l_num=0;
	f->_l_off=0;
	f->_off=0;
}



__SLL_EXTERNAL void sll_file_reset_line(sll_file_t* f,sll_file_offset_t off){
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (off>=f->dt.mm.sz){
			f->_l_num=0;
			f->_l_off=0;
			f->_off=f->dt.mm.sz;
			return;
		}
		const sll_char_t* dt=f->dt.mm.p;
		while (off&&dt[off]!='\n'){
			off--;
		}
		if (dt[off]=='\n'){
			off++;
		}
		f->_off=off;
		return;
	}
	sll_platform_file_seek(f->dt.fl.fd,off);
	sll_char_t c;
	if (!sll_platform_file_read(f->dt.fl.fd,&c,sizeof(sll_char_t))){
		f->_l_num=0;
		f->_l_off=0;
		f->_off=0;
		return;
	}
	while (off&&c!='\n'){
		off--;
		sll_platform_file_seek(f->dt.fl.fd,off);
		if (!sll_platform_file_read(f->dt.fl.fd,&c,sizeof(sll_char_t))){
			f->_l_num=0;
			f->_l_off=0;
			f->_off=0;
			return;
		}
	}
	if (c!='\n'){
		sll_platform_file_seek(f->dt.fl.fd,off);
	}
	else{
		off++;
	}
	f->_off=off;
	if (!(f->f&SLL_FILE_FLAG_NO_BUFFER)){
		f->_r_bf_off=0;
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE);
	}
}



__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* p,sll_size_t sz){
	if (!(f->f&SLL_FILE_FLAG_WRITE)||!sz){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off+sz>=f->dt.mm.sz){
			sz=f->dt.mm.sz-f->_off;
		}
		sll_copy_data(p,sz,((sll_char_t*)(f->dt.mm.p))+f->_off);
		f->_off+=sz;
		return sz;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		return sll_platform_file_write(f->dt.fl.fd,p,sz);
	}
	if (sz+f->_w_bf_off<=FILE_BUFFER_SIZE){
		sll_copy_data(p,sz,f->_w_bf+f->_w_bf_off);
		f->_w_bf_off+=sz;
		if (f->_w_bf_off==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->dt.fl.fd,f->_w_bf,FILE_BUFFER_SIZE);
			f->_w_bf_off=0;
		}
		return sz;
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_w_bf_off;
	sll_copy_data(p,i,f->_w_bf+f->_w_bf_off);
	sll_platform_file_write(f->dt.fl.fd,f->_w_bf,FILE_BUFFER_SIZE);
	const sll_char_t* v=((const sll_char_t*)p)+i;
	sll_size_t o=sz;
	sz-=i;
	i=sz&(FILE_BUFFER_SIZE-1);
	sz-=i;
	if (sz){
		sll_platform_file_write(f->dt.fl.fd,v,sz);
		v+=sz;
	}
	if (i){
		sll_copy_data(v,i,f->_w_bf);
	}
	f->_w_bf_off=i;
	return o;
}



__SLL_EXTERNAL sll_bool_t sll_file_write_char(sll_file_t* f,sll_char_t c){
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off+1>=f->dt.mm.sz){
			return 0;
		}
		*(((sll_char_t*)(f->dt.mm.p))+f->_off)=c;
		f->_off++;
		return 1;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		return sll_platform_file_write(f->dt.fl.fd,&c,sizeof(sll_char_t));
	}
	SLL_ASSERT(f->_w_bf_off+1<=FILE_BUFFER_SIZE);
	f->_w_bf[f->_w_bf_off]=c;
	f->_w_bf_off++;
	if (f->_w_bf_off==FILE_BUFFER_SIZE){
		sll_platform_file_write(f->dt.fl.fd,f->_w_bf,FILE_BUFFER_SIZE);
		f->_w_bf_off=0;
	}
	return 1;
}


__SLL_EXTERNAL sll_size_t sll_file_write_char_count(sll_file_t* f,sll_char_t c,sll_size_t n){
	if (!(f->f&SLL_FILE_FLAG_WRITE)||!n){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off+n>=f->dt.mm.sz){
			n=f->dt.mm.sz-f->_off;
		}
		sll_set_memory(((sll_char_t*)(f->dt.mm.p))+f->_off,n,c);
		f->_off+=n;
		return n;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t o=0;
		while (n){
			o+=sll_platform_file_write(f->dt.fl.fd,&c,sizeof(sll_char_t));
			n--;
		}
		return o;
	}
	if (n+f->_w_bf_off<=FILE_BUFFER_SIZE){
		sll_set_memory(f->_w_bf+f->_w_bf_off,n,c);
		f->_w_bf_off+=n;
		if (f->_w_bf_off==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->dt.fl.fd,f->_w_bf,FILE_BUFFER_SIZE);
			f->_w_bf_off=0;
		}
		return n;
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_w_bf_off;
	sll_set_memory(f->_w_bf+f->_w_bf_off,i,c);
	sll_platform_file_write(f->dt.fl.fd,f->_w_bf,FILE_BUFFER_SIZE);
	sll_size_t o=n;
	n-=i;
	i=n&(FILE_BUFFER_SIZE-1);
	n-=i;
	if (n){
		sll_set_memory(f->_w_bf,FILE_BUFFER_SIZE,c);
		while (n){
			sll_platform_file_write(f->dt.fl.fd,f->_w_bf,FILE_BUFFER_SIZE);
			n-=FILE_BUFFER_SIZE;
		}
	}
	if (i){
		sll_set_memory(f->_w_bf,i,c);
	}
	f->_w_bf_off=i;
	return o;
}



__SLL_EXTERNAL sll_size_t sll_file_write_format(sll_file_t* f,const sll_char_t* t,...){
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	va_list va;
	va_start(va,t);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_string_t str;
	sll_string_format_list(t,sll_string_length_unaligned(t),&dt,&str);
	va_end(va);
	sll_size_t o=sll_file_write(f,str.v,str.l);
	sll_free_string(&str);
	return o;
}



__SLL_EXTERNAL sll_size_t sll_file_write_string(sll_file_t* f,const sll_char_t* p){
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	return sll_file_write(f,p,sll_string_length_unaligned(p));
}

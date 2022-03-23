#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/file.h>
#include <sll/api/hash.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/platform/file.h>
#include <sll/platform/lock.h>
#include <sll/platform/memory.h>
#include <sll/platform/path.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#ifdef DEBUG_BUILD
#define EXTRA_FLAGS SLL_FILE_FLAG_NO_BUFFER
#else
#define EXTRA_FLAGS 0
#endif



#define LOCK SLL_CRITICAL(sll_platform_lock_acquire(f->_lck))
#define UNLOCK SLL_CRITICAL(sll_platform_lock_release(f->_lck))



static sll_file_t _file_stdin;
static sll_file_t _file_stdout;
static sll_file_t _file_stderr;



__SLL_EXTERNAL sll_file_t* sll_stdin=&_file_stdin;
__SLL_EXTERNAL sll_file_t* sll_stdout=&_file_stdout;
__SLL_EXTERNAL sll_file_t* sll_stderr=&_file_stderr;



void _file_end_hash(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_READ)||f->_h.bfl==DISABLE_FILE_HASH){
		return;
	}
	sll_char_t tmp[128];
	sll_set_memory(tmp,128,0);
	sll_copy_data(f->_h.bf,f->_h.bfl,tmp);
	__SLL_U8 offset=(f->_h.bfl<56?56:120);
	tmp[f->_h.bfl]=128;
	tmp[offset]=(f->_off>>53)&0xff;
	tmp[offset+1]=(f->_off>>45)&0xff;
	tmp[offset+2]=(f->_off>>37)&0xff;
	tmp[offset+3]=(f->_off>>29)&0xff;
	tmp[offset+4]=(f->_off>>21)&0xff;
	tmp[offset+5]=(f->_off>>13)&0xff;
	tmp[offset+6]=(f->_off>>5)&0xff;
	tmp[offset+7]=(f->_off<<3)&0xff;
	sll_hash_sha256(&(f->_h.h),tmp,(f->_h.bfl<56?64:128));
}



void _file_init_std_streams(void){
	sll_file_open_descriptor(SLL_CHAR("sll_stdin"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_INPUT),SLL_FILE_FLAG_READ|SLL_FILE_FLAG_NO_BUFFER,sll_stdin);
	sll_file_open_descriptor(SLL_CHAR("sll_stdout"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_OUTPUT),SLL_FILE_FLAG_WRITE|EXTRA_FLAGS,sll_stdout);
	sll_file_open_descriptor(SLL_CHAR("sll_stderr"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR),SLL_FILE_FLAG_WRITE|EXTRA_FLAGS,sll_stderr);
	(*((sll_file_flags_t*)(&(sll_stdin->f))))|=FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stdout->f))))|=FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stderr->f))))|=FILE_FLAG_NO_RELEASE;
}



void _file_release_std_streams(void){
	(*((sll_file_flags_t*)(&(sll_stdin->f))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stdout->f))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stderr->f))))&=~FILE_FLAG_NO_RELEASE;
	sll_file_close(sll_stdin);
	sll_file_close(sll_stdout);
	sll_file_close(sll_stderr);
}



void _file_start_hash(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_READ)||(!(f->f&FILE_FLAG_MEMORY)&&f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR)){
		return;
	}
	SLL_INIT_SHA256(&(f->_h.h));
	f->_h.bfl=0;
}



__SLL_EXTERNAL void sll_file_close(sll_file_t* f){
	sll_file_flush(f);
	if (f->f&FILE_FLAG_NO_RELEASE){
		return;
	}
	if (!(f->f&FILE_FLAG_MEMORY)){
		sll_free_string((sll_string_t*)(&(f->dt.fl.nm)));
		if (f->dt.fl.fd!=SLL_UNKNOWN_FILE_DESCRIPTOR){
			sll_platform_file_close(f->dt.fl.fd);
			if (!(f->f&SLL_FILE_FLAG_NO_BUFFER)){
				if (f->f&SLL_FILE_FLAG_READ){
					sll_platform_free_page(f->_r_bf,SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE));
				}
				if (f->f&SLL_FILE_FLAG_WRITE){
					sll_platform_free_page(f->_w.bf.p,SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE));
				}
			}
		}
	}
	else{
		SLL_ASSERT(f->f&SLL_FILE_FLAG_NO_BUFFER);
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* c=f->_w.d.b;
			do{
				dynamic_buffer_chunk_t* n=c->n;
				sll_platform_free_page(c,c->sz);
				c=n;
			} while (c);
		}
		sll_deallocate(PTR(f->dt.mm.p));
	}
	*((sll_file_flags_t*)(&(f->f)))=FILE_FLAG_NO_RELEASE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_data_available(sll_file_t* f){
	if ((f->f&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_ASYNC))!=(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_ASYNC)){
		return 1;
	}
	return sll_platform_file_data_available(f->dt.fl.fd);
}



__SLL_EXTERNAL sll_error_t sll_file_flush(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_WRITE)||(f->f&SLL_FILE_FLAG_NO_BUFFER)||!f->_w.bf.off){
		return SLL_NO_ERROR;
	}
	sll_error_t err=SLL_NO_ERROR;
	sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,f->_w.bf.off,&err);
	f->_w.bf.off=0;
	return err;
}



__SLL_EXTERNAL void sll_file_from_data(void* p,sll_size_t sz,sll_file_flags_t f,sll_file_t* o){
	f&=~FILE_FLAG_DYNAMIC_BUFFERS;
	if (f&SLL_FILE_FLAG_APPEND){
		SLL_UNIMPLEMENTED();
	}
	if (f&SLL_FILE_FLAG_WRITE){
		f|=FILE_FLAG_DYNAMIC_BUFFERS;
		dynamic_buffer_chunk_t* bf=sll_platform_allocate_page(SLL_ROUND_PAGE(FILE_DYNAMIC_BUFFER_ALLOC_SIZE),0);
		bf->sz=SLL_ROUND_PAGE(FILE_DYNAMIC_BUFFER_ALLOC_SIZE);
		bf->n=NULL;
		o->_w.d.b=bf;
		o->_w.d.t=bf;
		o->_w.d.sz=0;
		o->_w.d.off=0;
	}
	void* n=sll_allocate(sz);
	sll_copy_data(p,sz,n);
	*((void**)(&(o->dt.mm.p)))=n;
	*((sll_size_t*)(&(o->dt.mm.sz)))=sz;
	*((sll_file_flags_t*)(&(o->f)))=(f&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|FILE_FLAG_DYNAMIC_BUFFERS))|SLL_FILE_FLAG_NO_BUFFER|FILE_FLAG_MEMORY;
	o->_l_num=0;
	o->_off=0;
	o->_h.bfl=DISABLE_FILE_HASH;
	o->_lck=sll_platform_lock_create();
}



__SLL_EXTERNAL void sll_file_get_buffer(sll_file_t* f,sll_string_t* o){
	if (!(f->f&FILE_FLAG_DYNAMIC_BUFFERS)){
		SLL_INIT_STRING(o);
		return;
	}
	sll_string_create((sll_string_length_t)(f->_w.d.sz),o);
	dynamic_buffer_chunk_t* c=f->_w.d.b;
	addr_t ptr=ADDR(o->v);
	sll_string_length_t l=o->l;
	do{
		sll_copy_data(c->dt,(l>c->sz?c->sz:l),PTR(ptr));
		l-=(sll_string_length_t)(c->sz);
		ptr+=c->sz;
		c=c->n;
	} while (c);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL sll_error_t sll_file_open(const sll_char_t* nm,sll_file_flags_t f,sll_file_t* o){
	if ((f&SLL_FILE_FLAG_READ)&&!sll_platform_path_exists(nm)){
		return SLL_ERROR_NO_FILE_PATH;
	}
	sll_error_t err=SLL_NO_ERROR;
	sll_file_descriptor_t fd=sll_platform_file_open(nm,f,&err);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return err;
	}
	sll_file_open_descriptor(nm,fd,f,o);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL void sll_file_open_descriptor(const sll_char_t* nm,sll_file_descriptor_t fd,sll_file_flags_t f,sll_file_t* o){
	*((sll_file_descriptor_t*)(&(o->dt.fl.fd)))=fd;
	sll_string_from_pointer(nm,(sll_string_t*)(&(o->dt.fl.nm)));
	*((sll_file_flags_t*)(&(o->f)))=f&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		*((sll_file_flags_t*)(&(o->f)))=f&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE);
		return;
	}
	if (sll_platform_file_async_read(fd)){
		*((sll_file_flags_t*)(&(o->f)))|=SLL_FILE_FLAG_ASYNC;
	}
	o->_l_num=0;
	o->_off=0;
	if (!(f&SLL_FILE_FLAG_NO_BUFFER)){
		if (f&SLL_FILE_FLAG_READ){
			o->_r_bf=sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE),1);
			o->_r_bf_off=0;
			o->_r_bf_sz=0;
		}
		if (f&SLL_FILE_FLAG_WRITE){
			o->_w.bf.p=sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE),1);
			o->_w.bf.off=0;
		}
	}
	o->_h.bfl=DISABLE_FILE_HASH;
	o->_lck=sll_platform_lock_create();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_peek_char(sll_file_t* f,sll_error_t* err){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		if (f->_off==f->dt.mm.sz){
			return SLL_END_OF_DATA;
		}
		sll_char_t o=*((sll_char_t*)(PTR(ADDR(f->dt.mm.p)+f->_off)));
		UNLOCK;
		return o;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return SLL_END_OF_DATA;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		SLL_UNIMPLEMENTED();
	}
	LOCK;
	if (!f->_r_bf_off&&!f->_r_bf_sz){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		if (!f->_r_bf_sz){
			return SLL_END_OF_DATA;
		}
	}
	sll_char_t o=f->_r_bf[f->_r_bf_off];
	UNLOCK;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* p,sll_size_t sz,sll_error_t* err){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_READ)||!sz){
		return 0;
	}
	sll_size_t o;
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		if (f->_off+sz>=f->dt.mm.sz){
			sz=f->dt.mm.sz-f->_off;
		}
		sll_copy_data(PTR(ADDR(f->dt.mm.p)+f->_off),sz,p);
		f->_off+=sz;
		o=sz;
		goto _hash_data;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	LOCK;
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sz=sll_platform_file_read(f->dt.fl.fd,p,sz,err);
		f->_off+=sz;
		o=sz;
		goto _hash_data;
	}
	if (!f->_r_bf_off&&!f->_r_bf_sz){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		if (!f->_r_bf_sz){
			UNLOCK;
			return 0;
		}
	}
	if (f->_r_bf_off+sz<=f->_r_bf_sz){
		sll_copy_data(f->_r_bf+f->_r_bf_off,sz,p);
		f->_r_bf_off+=sz;
		if (f->_r_bf_off==f->_r_bf_sz){
			f->_r_bf_off=0;
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		}
		o=sz;
		goto _hash_data;
	}
	o=f->_r_bf_sz-f->_r_bf_off;
	sz-=o;
	sll_copy_data(f->_r_bf+f->_r_bf_off,o,p);
	p=PTR(ADDR(p)+o);
	while (sz>FILE_BUFFER_SIZE){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,p,FILE_BUFFER_SIZE,err);
		if (f->_r_bf_sz!=FILE_BUFFER_SIZE){
			SLL_UNIMPLEMENTED();
			f->_r_bf_off=0;
			f->_r_bf_sz=0;
			goto _hash_data;
		}
		o+=FILE_BUFFER_SIZE;
		sz-=FILE_BUFFER_SIZE;
		p=PTR(ADDR(p)+FILE_BUFFER_SIZE);
	}
	f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
	if (f->_r_bf_sz<sz){
		o+=f->_r_bf_sz;
		sll_copy_data(f->_r_bf,f->_r_bf_sz,p);
		f->_r_bf_off=0;
		f->_r_bf_sz=0;
		goto _hash_data;
	}
	o+=sz;
	f->_r_bf_off=sz;
	sll_copy_data(f->_r_bf,sz,p);
_hash_data:
	if (f->_h.bfl!=DISABLE_FILE_HASH){
		SLL_UNIMPLEMENTED();
	}
	UNLOCK;
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_read_char_t sll_file_read_char(sll_file_t* f,sll_error_t* err){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	LOCK;
	sll_char_t o;
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off==f->dt.mm.sz){
			return SLL_END_OF_DATA;
		}
		o=*((sll_char_t*)(PTR(ADDR(f->dt.mm.p)+f->_off)));
	}
	else if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return SLL_END_OF_DATA;
	}
	else if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		if (!sll_platform_file_read(f->dt.fl.fd,&o,sizeof(sll_char_t),err)){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
	}
	else{
		if (!f->_r_bf_off&&!f->_r_bf_sz){
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
			if (!f->_r_bf_sz){
				UNLOCK;
				return SLL_END_OF_DATA;
			}
		}
		o=f->_r_bf[f->_r_bf_off];
		f->_r_bf_off++;
		if (f->_r_bf_off==f->_r_bf_sz){
			f->_r_bf_off=0;
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		}
	}
	f->_off++;
	if (o=='\n'){
		f->_l_num++;
	}
	if (f->_h.bfl!=DISABLE_FILE_HASH){
		f->_h.bf[f->_h.bfl]=o;
		if (f->_h.bfl==63){
			f->_h.bfl=0;
			sll_hash_sha256(&(f->_h.h),f->_h.bf,64);
		}
		else{
			f->_h.bfl++;
		}
	}
	UNLOCK;
	return DATA_IF_ERROR_PTR(o,SLL_END_OF_DATA);
}



__SLL_EXTERNAL sll_error_t sll_file_reset(sll_file_t* f){
	sll_error_t err=SLL_NO_ERROR;
	LOCK;
	if (!(f->f&FILE_FLAG_MEMORY)){
		if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
			UNLOCK;
			return SLL_ERROR_UNKNOWN_FD;
		}
		err=sll_platform_file_seek(f->dt.fl.fd,0);
		if (err!=SLL_NO_ERROR){
			UNLOCK;
			return err;
		}
		f->_r_bf_off=0;
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,&err);
	}
	f->_l_num=0;
	f->_off=0;
	if (f->_h.bfl!=DISABLE_FILE_HASH){
		_file_start_hash(f);
	}
	UNLOCK;
	return err;
}



__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* p,sll_size_t sz,sll_error_t* err){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_WRITE)||!sz){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* c=f->_w.d.t;
			if (c->sz-f->_w.d.off>=sz){
				sll_copy_data(p,sz,c->dt+f->_w.d.off);
				f->_w.d.sz+=sz;
				f->_w.d.off+=sz;
				UNLOCK;
				return sz;
			}
			SLL_UNIMPLEMENTED();
			UNLOCK;
			return sz;
		}
		if (f->_off+sz>=f->dt.mm.sz){
			sz=f->dt.mm.sz-f->_off;
		}
		sll_copy_data(p,sz,((sll_char_t*)(f->dt.mm.p))+f->_off);
		f->_off+=sz;
		UNLOCK;
		return sz;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	LOCK;
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t o=sll_platform_file_write(f->dt.fl.fd,p,sz,err);
		UNLOCK;
		return o;
	}
	if (sz+f->_w.bf.off<=FILE_BUFFER_SIZE){
		sll_copy_data(p,sz,f->_w.bf.p+f->_w.bf.off);
		f->_w.bf.off+=sz;
		if (f->_w.bf.off==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
			f->_w.bf.off=0;
		}
		UNLOCK;
		return ZERO_IF_ERROR_PTR(sz);
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_w.bf.off;
	sll_copy_data(p,i,f->_w.bf.p+f->_w.bf.off);
	sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
	const sll_char_t* v=((const sll_char_t*)p)+i;
	sll_size_t o=sz;
	sz-=i;
	i=sz&(FILE_BUFFER_SIZE-1);
	sz-=i;
	if (sz){
		sll_platform_file_write(f->dt.fl.fd,v,sz,err);
		v+=sz;
	}
	if (i){
		sll_copy_data(v,i,f->_w.bf.p);
	}
	f->_w.bf.off=i;
	UNLOCK;
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_bool_t sll_file_write_char(sll_file_t* f,sll_char_t c,sll_error_t* err){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* k=f->_w.d.t;
			if (k->sz>f->_w.d.off){
				k->dt[f->_w.d.off]=c;
				f->_w.d.sz++;
				f->_w.d.off++;
				UNLOCK;
				return 1;
			}
			SLL_UNIMPLEMENTED();
			UNLOCK;
			return 1;
		}
		if (f->_off+1>=f->dt.mm.sz){
			UNLOCK;
			return 0;
		}
		*(((sll_char_t*)(f->dt.mm.p))+f->_off)=c;
		f->_off++;
		UNLOCK;
		return 1;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	LOCK;
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sll_bool_t o=sll_platform_file_write(f->dt.fl.fd,&c,sizeof(sll_char_t),err);
		UNLOCK;
		return o;
	}
	SLL_ASSERT(f->_w.bf.off+1<=FILE_BUFFER_SIZE);
	f->_w.bf.p[f->_w.bf.off]=c;
	f->_w.bf.off++;
	if (f->_w.bf.off==FILE_BUFFER_SIZE){
		sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
		f->_w.bf.off=0;
	}
	UNLOCK;
	return ZERO_IF_ERROR_PTR(1);
}



__SLL_EXTERNAL sll_size_t sll_file_write_char_count(sll_file_t* f,sll_char_t c,sll_size_t n,sll_error_t* err){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_WRITE)||!n){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			SLL_UNIMPLEMENTED();
		}
		LOCK;
		if (f->_off+n>=f->dt.mm.sz){
			n=f->dt.mm.sz-f->_off;
		}
		sll_set_memory(((sll_char_t*)(f->dt.mm.p))+f->_off,n,c);
		f->_off+=n;
		UNLOCK;
		return n;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	LOCK;
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t o=0;
		while (n){
			o+=sll_platform_file_write(f->dt.fl.fd,&c,sizeof(sll_char_t),err);
			n--;
		}
		UNLOCK;
		return ZERO_IF_ERROR_PTR(o);
	}
	if (n+f->_w.bf.off<=FILE_BUFFER_SIZE){
		sll_set_memory(f->_w.bf.p+f->_w.bf.off,n,c);
		f->_w.bf.off+=n;
		if (f->_w.bf.off==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
			f->_w.bf.off=0;
		}
		UNLOCK;
		return ZERO_IF_ERROR_PTR(n);
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_w.bf.off;
	sll_set_memory(f->_w.bf.p+f->_w.bf.off,i,c);
	sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
	sll_size_t o=n;
	n-=i;
	i=n&(FILE_BUFFER_SIZE-1);
	n-=i;
	if (n){
		sll_set_memory(f->_w.bf.p,FILE_BUFFER_SIZE,c);
		while (n){
			sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
			n-=FILE_BUFFER_SIZE;
		}
	}
	if (i){
		sll_set_memory(f->_w.bf.p,i,c);
	}
	f->_w.bf.off=i;
	UNLOCK;
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_size_t sll_file_write_format(sll_file_t* f,const sll_char_t* t,sll_error_t* err,...){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (!(f->f&FILE_FLAG_MEMORY)&&f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	va_list va;
	va_start(va,err);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_string_t str;
	sll_string_format_list(t,sll_string_length_unaligned(t),&dt,&str);
	va_end(va);
	sll_size_t o=sll_file_write(f,str.v,str.l,err);
	sll_free_string(&str);
	return o;
}



__SLL_EXTERNAL sll_size_t sll_file_write_string(sll_file_t* f,const sll_char_t* p,sll_error_t* err){
	RESET_ERROR_PTR;
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (!(f->f&FILE_FLAG_MEMORY)&&f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	return sll_file_write(f,p,sll_string_length_unaligned(p),err);
}

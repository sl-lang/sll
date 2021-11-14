#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/init.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



static sll_file_t _file_stdin;
static sll_file_t _file_stdout;
static sll_file_t _file_stderr;



__SLL_EXTERNAL sll_file_t* sll_stdin=&_file_stdin;
__SLL_EXTERNAL sll_file_t* sll_stdout=&_file_stdout;
__SLL_EXTERNAL sll_file_t* sll_stderr=&_file_stderr;



void _file_init_std_streams(void){
	*((sll_file_descriptor_t*)(&(_file_stdin.fd)))=_platform_get_stream_descriptor(0);
	sll_string_from_pointer(SLL_CHAR("sll_stdin"),(sll_string_t*)(&(_file_stdin.nm)));
	*((sll_file_flags_t*)(&(_file_stdin.f)))=SLL_FILE_FLAG_READ|SLL_FILE_FLAG_NO_BUFFER;
	_file_stdin._l_num=0;
	_file_stdin._l_off=0;
	_file_stdin._off=0;
	*((sll_file_descriptor_t*)(&(_file_stdout.fd)))=_platform_get_stream_descriptor(1);
	sll_string_from_pointer(SLL_CHAR("sll_stdout"),(sll_string_t*)(&(_file_stdout.nm)));
	*((sll_file_flags_t*)(&(_file_stdout.f)))=SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_NO_BUFFER;
	*((sll_file_descriptor_t*)(&(_file_stderr.fd)))=_platform_get_stream_descriptor(2);
	sll_string_from_pointer(SLL_CHAR("sll_stderr"),(sll_string_t*)(&(_file_stderr.nm)));
	*((sll_file_flags_t*)(&(_file_stderr.f)))=SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_NO_BUFFER;
}



void _file_release_std_streams(void){
	sll_deinit_string((sll_string_t*)(&(sll_stdin->nm)));
	sll_deinit_string((sll_string_t*)(&(sll_stdout->nm)));
	sll_deinit_string((sll_string_t*)(&(sll_stderr->nm)));
}



__SLL_EXTERNAL void sll_file_close(sll_file_t* f){
	sll_file_flush(f);
	sll_platform_file_close(f->fd);
	sll_deinit_string((sll_string_t*)(&(f->nm)));
}



__SLL_EXTERNAL void sll_file_flush(sll_file_t* f){
	if ((f->f&(SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_NO_BUFFER))!=(SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_NO_BUFFER)){
		return;
	}
}



__SLL_EXTERNAL void sll_file_from_data(void* p,sll_size_t sz,sll_file_flags_t f,sll_file_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_open(const sll_char_t* nm,sll_file_flags_t f,sll_file_t* o){
	if ((f&SLL_FILE_FLAG_READ)&&!sll_platform_path_exists(nm)){
		return 0;
	}
	sll_file_descriptor_t fd=sll_platform_file_open(nm,f);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return 0;
	}
	*((sll_file_descriptor_t*)(&(o->fd)))=fd;
	sll_string_from_pointer(nm,(sll_string_t*)(&(o->nm)));
	*((sll_file_flags_t*)(&(o->f)))=f;
	if (f&SLL_FILE_FLAG_READ){
		o->_l_num=0;
		o->_l_off=0;
		o->_off=0;
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* p,sll_size_t sz){
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return 0;
	}
	return sll_platform_file_read(f->fd,p,sz);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_read_char(sll_file_t* f){
	sll_char_t o;
	if (!sll_file_read(f,&o,sizeof(sll_char_t))){
		return SLL_END_OF_DATA;
	}
	f->_off++;
	if (o=='\n'){
		f->_l_num++;
		f->_l_off=f->_off;
	}
	return o;
}



__SLL_EXTERNAL void sll_file_reset(sll_file_t* f){
	sll_platform_file_seek(f->fd,0);
	f->_l_num=0;
	f->_l_off=0;
	f->_off=0;
}



__SLL_EXTERNAL void sll_file_reset_line(sll_file_t* f,sll_file_offset_t off){
	sll_platform_file_seek(f->fd,off);
	sll_char_t c;
	if (!sll_platform_file_read(f->fd,&c,sizeof(sll_char_t))){
		f->_l_num=0;
		f->_l_off=0;
		f->_off=0;
		return;
	}
	while (off&&c!='\n'&&c!='\r'){
		off--;
		sll_platform_file_seek(f->fd,off);
		if (!sll_platform_file_read(f->fd,&c,sizeof(sll_char_t))){
			f->_l_num=0;
			f->_l_off=0;
			f->_off=0;
			return;
		}
	}
	if (c!='\n'&&c!='\r'){
		sll_platform_file_seek(f->fd,off);
	}
	else{
		off++;
	}
	f->_off=off;
}



__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* p,sll_size_t sz){
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	return sll_platform_file_write(f->fd,p,sz);
}



__SLL_EXTERNAL void sll_file_write_char(sll_file_t* f,sll_char_t c){
	sll_platform_file_write(f->fd,&c,sizeof(sll_char_t));
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>



__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd){
	close((int)(uint64_t)fd);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* fp,sll_file_flags_t ff){
	int fl=O_RDONLY;
	if ((ff&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE))==(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE)){
		fl=O_RDWR;
	}
	else if (ff&SLL_FILE_FLAG_WRITE){
		fl=O_WRONLY;
	}
	if (ff&SLL_FILE_FLAG_APPEND){
		fl|=O_APPEND;
	}
	else if (ff&SLL_FILE_FLAG_WRITE){
		fl|=O_CREAT|O_TRUNC;
	}
	int o=open((char*)fp,fl,S_IRWXU|S_IRWXG|S_IRWXO);
	return (o==-1?SLL_UNKNOWN_FILE_DESCRIPTOR:((sll_file_descriptor_t)(uint64_t)o));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* p,sll_size_t sz){
	ssize_t o=read((int)(uint64_t)fd,p,sz);
	return (o==-1?0:o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_size(sll_file_descriptor_t fd){
	struct stat st;
	if (!fstat((int)(uint64_t)fd,&st)){
		return st.st_size;
	}
	return 0;
}



__SLL_EXTERNAL void sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t off){
	lseek((int)(uint64_t)fd,off,SEEK_SET);
}



__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz){
	ssize_t o=write((int)(uint64_t)fd,p,sz);
	return (o==-1?0:o);
}



sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t t){
	if (t==SLL_PLATFORM_STREAM_INPUT){
		return (sll_file_descriptor_t)(uint64_t)STDIN_FILENO;
	}
	if (t==SLL_PLATFORM_STREAM_OUTPUT){
		return (sll_file_descriptor_t)(uint64_t)STDOUT_FILENO;
	}
	SLL_ASSERT(t==SLL_PLATFORM_STREAM_ERROR);
	return (sll_file_descriptor_t)(uint64_t)STDERR_FILENO;
}

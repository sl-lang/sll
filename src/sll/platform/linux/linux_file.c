#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/platform/file.h>
#include <sll/types.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <unistd.h>



#define FROM_HANDLE(fd) ((int)ADDR(fd))
#define TO_HANDLE(fd) ((sll_file_descriptor_t)PTR(fd))



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_async_read(sll_file_descriptor_t fd){
	struct stat bf;
	if (fstat(FROM_HANDLE(fd),&bf)==-1){
		return 0;
	}
	mode_t m=bf.st_mode&S_IFMT;
	return (m!=S_IFDIR&&m!=S_IFREG);
}



__SLL_EXTERNAL void sll_platform_file_close(sll_file_descriptor_t fd){
	close(FROM_HANDLE(fd));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_data_available(sll_file_descriptor_t fd){
	struct pollfd bf={
		FROM_HANDLE(fd),
		POLLIN
	};
	return !!poll(&bf,1,0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* fp,sll_file_flags_t ff,sll_error_t* err){
	RESET_ERROR_PTR;
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
	SET_ERROR_IF_LIBC_FAIL_PTR(o);
	return (o==-1?SLL_UNKNOWN_FILE_DESCRIPTOR:TO_HANDLE(o));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* p,sll_size_t sz,sll_error_t* err){
	RESET_ERROR_PTR;
	ssize_t o=read(FROM_HANDLE(fd),p,sz);
	if (o==-1){
		LIBC_ERROR_PTR;
		return SLL_NO_FILE_SIZE;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_size(sll_file_descriptor_t fd,sll_error_t* err){
	RESET_ERROR_PTR;
	struct stat st;
	if (!fstat(FROM_HANDLE(fd),&st)){
		return st.st_size;
	}
	LIBC_ERROR_PTR;
	return SLL_NO_FILE_SIZE;
}



__SLL_EXTERNAL sll_error_t sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t off){
	return (lseek(FROM_HANDLE(fd),off,SEEK_SET)==-1?LIBC_ERROR:SLL_NO_ERROR);
}



__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* p,sll_size_t sz,sll_error_t* err){
	RESET_ERROR_PTR;
	ssize_t o=write(FROM_HANDLE(fd),p,sz);
	if (o==-1){
		LIBC_ERROR_PTR;
		return SLL_NO_FILE_SIZE;
	}
	return o;
}



sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t t){
	if (t==SLL_PLATFORM_STREAM_INPUT){
		return TO_HANDLE(dup(STDIN_FILENO));
	}
	if (t==SLL_PLATFORM_STREAM_OUTPUT){
		return TO_HANDLE(dup(STDOUT_FILENO));
	}
	SLL_ASSERT(t==SLL_PLATFORM_STREAM_ERROR);
	return TO_HANDLE(dup(STDERR_FILENO));
}

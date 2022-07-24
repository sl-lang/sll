#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/platform/file.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <unistd.h>



#define FROM_HANDLE(fd) ((int)ADDR(fd))
#define TO_HANDLE(fd) ((sll_file_descriptor_t)PTR(fd))



#ifdef __SLL_BUILD_FUZZER
static int _fuzzer_fileno=STDERR_FILENO;
__SLL_EXTERNAL void __sll_fuzzer_set_fd(int fd){
	_fuzzer_fileno=fd;
}
#endif



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_async_read(sll_file_descriptor_t fd){
	struct stat bf;
	if (fstat(FROM_HANDLE(fd),&bf)==-1){
		return 0;
	}
	mode_t m=bf.st_mode&S_IFMT;
	return (m!=S_IFDIR&&m!=S_IFREG);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_file_close(sll_file_descriptor_t fd){
	return (close(FROM_HANDLE(fd))?sll_platform_get_error():SLL_NO_ERROR);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_file_data_available(sll_file_descriptor_t fd){
	struct pollfd bf={
		FROM_HANDLE(fd),
		POLLIN
	};
	return !!poll(&bf,1,0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_descriptor_t sll_platform_file_open(const sll_char_t* file_path,sll_file_flags_t flags,sll_error_t* err){
	ERROR_PTR_RESET;
	int fl=O_RDONLY;
	if ((flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE))==(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE)){
		fl=O_RDWR;
	}
	else if (flags&SLL_FILE_FLAG_WRITE){
		fl=O_WRONLY;
	}
	if (flags&SLL_FILE_FLAG_APPEND){
		fl|=O_APPEND;
	}
	else if (flags&SLL_FILE_FLAG_WRITE){
		fl|=O_CREAT|O_TRUNC;
	}
	int o=open((char*)file_path,fl,S_IRWXU|S_IRWXG|S_IRWXO);
	if (o==-1){
		ERROR_PTR_SYSTEM;
		return SLL_UNKNOWN_FILE_DESCRIPTOR;
	}
	return TO_HANDLE(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_read(sll_file_descriptor_t fd,void* pointer,sll_size_t size,sll_error_t* err){
	ERROR_PTR_RESET;
	ssize_t o=read(FROM_HANDLE(fd),pointer,size);
	if (o==-1){
		ERROR_PTR_SYSTEM;
		return SLL_NO_FILE_SIZE;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_file_size(sll_file_descriptor_t fd,sll_error_t* err){
	ERROR_PTR_RESET;
	struct stat st;
	if (!fstat(FROM_HANDLE(fd),&st)){
		return st.st_size;
	}
	ERROR_PTR_SYSTEM;
	return SLL_NO_FILE_SIZE;
}



__SLL_EXTERNAL sll_error_t sll_platform_file_seek(sll_file_descriptor_t fd,sll_file_offset_t offset){
	return (lseek(FROM_HANDLE(fd),offset,SEEK_SET)==-1?sll_platform_get_error():SLL_NO_ERROR);
}



__SLL_EXTERNAL sll_size_t sll_platform_file_write(sll_file_descriptor_t fd,const void* pointer,sll_size_t size,sll_error_t* err){
	ERROR_PTR_RESET;
	ssize_t o=write(FROM_HANDLE(fd),pointer,size);
	if (o==-1){
		ERROR_PTR_SYSTEM;
		return SLL_NO_FILE_SIZE;
	}
	return o;
}



sll_file_descriptor_t sll_platform_get_default_stream_descriptor(sll_char_t type){
	if (type==SLL_PLATFORM_STREAM_INPUT){
		return TO_HANDLE(dup(STDIN_FILENO));
	}
#ifdef __SLL_BUILD_FUZZER
	return TO_HANDLE(dup(_fuzzer_fileno));
#endif
	if (type==SLL_PLATFORM_STREAM_OUTPUT){
		return TO_HANDLE(dup(STDOUT_FILENO));
	}
	SLL_ASSERT(type==SLL_PLATFORM_STREAM_ERROR);
	return TO_HANDLE(dup(STDERR_FILENO));
}

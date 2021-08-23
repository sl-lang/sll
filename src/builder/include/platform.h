#ifndef __PLATFORM_H__
#define __PLATFORM_H__ 1
#include <stdint.h>



#define LIST_FILES_ONLY_FILES NULL



typedef uint8_t (*list_files_callback_t)(const char* fp,void* ctx);



void copy_file(const char* s,const char* d);



void create_build(void);



void list_files(const char* fp,list_files_callback_t d_cb,list_files_callback_t f_cb,void* ctx);



#endif

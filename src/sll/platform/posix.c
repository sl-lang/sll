#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <dirent.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>



const sll_char_t* sll_platform_string=SLL_CHAR("posix");



static void _list_dir_files(sll_char_t* bf,sll_string_length_t i,file_list_data_t* o){
	bf[i]=0;
	DIR* d=opendir((char*)bf);
	if (d){
		struct dirent* dt;
		while ((dt=readdir(d))){
			if (dt->d_type==DT_REG){
				sll_string_length_t j=sll_string_length_unaligned(SLL_CHAR(dt->d_name));
				if (!(o->l&7)){
					o->dt=sll_reallocate(o->dt,(o->l+8)*sizeof(sll_string_t));
				}
				sll_string_t* s=o->dt+o->l;
				o->l++;
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



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_page_size_t sz){
	return mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_integer_t sll_platform_file_size(const sll_char_t* fp){
	struct stat st;
	if (!stat((char*)fp,&st)){
		return st.st_size;
	}
	return 0;
}



__SLL_FUNC void sll_platform_free_page(void* pg,sll_page_size_t sz){
	munmap(pg,sz);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_page_size(void){
	return sysconf(_SC_PAGESIZE);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o){
	DIR* d=opendir((char*)fp);
	sll_string_t* op=NULL;
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



__SLL_FUNC __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o){
	sll_char_t bf[PATH_MAX+1];
	sll_string_length_t l=sll_string_length_unaligned(fp);
	sll_copy_data(fp,l,bf);
	if (bf[l-1]!='/'&&bf[l-1]!='\\'){
		bf[l]='/';
		l++;
	}
	file_list_data_t dt={
		NULL,
		0
	};
	_list_dir_files(bf,l,&dt);
	*o=sll_reallocate(dt.dt,dt.l*sizeof(sll_string_t));
	return dt.l;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_path_absolute(const sll_char_t* fp,sll_char_t* o,sll_string_length_t ol){
	IGNORE_RESULT(ol);
	return (!realpath((char*)fp,(char*)o)?0:sll_string_length_unaligned(SLL_CHAR(o)));
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp){
	return !access((char*)fp,F_OK);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp){
	struct stat st;
	return (!stat((char*)fp,&st)&&(st.st_mode&S_IFDIR));
}



__SLL_FUNC void sll_platform_setup_console(void){
}



__SLL_FUNC void sll_platform_sleep(sll_time_t tm){
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	sll_time_t e=ts.tv_sec*1000000000+ts.tv_nsec+tm;
	while (1){
		struct timeval tv;
		tv.tv_sec=tm/1000000000;
		tv.tv_usec=(tm/1000)%1000000;
		if (!select(0,NULL,NULL,NULL,&tv)){
			return;
		}
		clock_gettime(CLOCK_REALTIME,&ts);
		sll_time_t c=ts.tv_sec*1000000000+ts.tv_nsec;
		if (c>=e){
			break;
		}
		tm=c-e;
	}
}



__SLL_FUNC void sll_platform_socket_init(void){
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_platform_socket_execute(const sll_string_t* h,unsigned int p,const sll_string_t* in,sll_string_t* o){
	SLL_INIT_STRING(o);
	struct addrinfo ah;
	memset(&ah,0,sizeof(struct addrinfo));
	ah.ai_family=AF_UNSPEC;
	ah.ai_socktype=SOCK_STREAM;
	ah.ai_protocol=IPPROTO_TCP;
	struct addrinfo* r=NULL;
	char ps[6]={0,0,0,0,0,0};
	uint8_t i=4;
	do{
		ps[i]=(p%10)+48;
		i--;
		p/=10;
	} while (p);
	if (getaddrinfo((char*)h->v,ps+i+1,&ah,&r)){
		return SLL_RETURN_ERROR;
	}
	int s=-1;
	for (struct addrinfo* k=r;k;k=k->ai_next){
		s=socket(k->ai_family,k->ai_socktype,k->ai_protocol);
		if (s==-1){
			freeaddrinfo(r);
			return SLL_RETURN_ERROR;
		}
		if (!connect(s,k->ai_addr,(int)k->ai_addrlen)){
			break;
		}
		s=-1;
	}
	freeaddrinfo(r);
	if (s==-1){
		return SLL_RETURN_ERROR;
	}
	if (send(s,in->v,in->l,0)==-1){
		close(s);
		return SLL_RETURN_ERROR;
	}
	sll_string_create(0,o);
	sll_char_t bf[4096];
	int l=recv(s,bf,4096,0);
	shutdown(s,SHUT_WR);
	while (l){
		if (l<0){
			close(s);
			return SLL_RETURN_ERROR;
		}
		sll_string_increase(o,l);
		sll_copy_data(bf,l,o->v+o->l);
		o->l+=l;
		l=recv(s,bf,4096,0);
	};
	close(s);
	o->v[o->l]=0;
	sll_string_calculate_checksum(o);
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC void sll_platform_reset_console(void){
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
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



const char* sll_platform_string="posix";



void _list_dir_files(char* bf,uint16_t i,sll_string_checksum_t c,file_list_data_t* o){
	bf[i]=0;
	DIR* d=opendir(bf);
	if (d){
		struct dirent* dt;
		while ((dt=readdir(d))){
			if (dt->d_type==DT_REG){
				sll_string_checksum_t nc=c;
				uint16_t j=0;
				while (*(dt->d_name+j)){
					bf[i+j]=*(dt->d_name+j);
					nc^=bf[i+j];
					j++;
				}
				o->l++;
				o->dt=realloc(o->dt,o->l*sizeof(sll_string_t));
				sll_string_t* s=o->dt+o->l-1;
				sll_string_create(i+j,s);
				s->c=nc;
				memcpy(s->v,bf,i+j);
			}
			else if (dt->d_type==DT_DIR){
				if (*(dt->d_name)=='.'&&(*(dt->d_name+1)==0||(*(dt->d_name+1)=='.'&&*(dt->d_name+2)==0))){
					continue;
				}
				SLL_UNIMPLEMENTED();
			}
		}
		closedir(d);
	}
}



__SLL_FUNC void* sll_platform_allocate_page(sll_page_size_t sz){
	return mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
}



__SLL_FUNC void sll_platform_free_page(void* pg,sll_page_size_t sz){
	munmap(pg,sz);
}



__SLL_FUNC sll_time_t sll_platform_get_current_time(void){
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	return tm.tv_sec*1000000000+tm.tv_nsec;
}



__SLL_FUNC sll_time_t sll_platform_get_page_size(void){
	return sysconf(_SC_PAGESIZE);
}



__SLL_FUNC sll_array_length_t sll_platform_list_directory_recursive(const char* fp,sll_string_t** o){
	sll_string_checksum_t c=0;
	char bf[PATH_MAX];
	uint16_t i=0;
	while (*(fp+i)){
		bf[i]=*(fp+i);
		c^=bf[i];
		i++;
	}
	file_list_data_t dt={
		NULL,
		0
	};
	_list_dir_files(bf,i,c,&dt);
	*o=dt.dt;
	return dt.l;
}



__SLL_FUNC sll_buffer_size_t sll_platform_path_absolute(const char* fp,sll_buffer_t bf,sll_buffer_size_t bfl){
	return !!realpath(fp,(char*)bf);
}



__SLL_FUNC sll_bool_t sll_platform_path_exists(const char* fp){
	return !access(fp,F_OK);
}



__SLL_FUNC sll_bool_t sll_platform_path_is_directory(const char* fp){
	struct stat st;
	sll_bool_t o=0;
	if (!stat(fp,&st)&&(st.st_mode&S_IFDIR)){
		o=1;
	}
	return o;
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



__SLL_FUNC __SLL_RETURN sll_platform_socket_execute(const sll_string_t* h,unsigned int p,const sll_string_t* in,sll_string_t* o){
	SLL_ZERO_STRING(o);
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
	o->l=1;
	o->v=malloc(sizeof(sll_char_t));
	sll_char_t bf[4096];
	int l=recv(s,bf,4096,0);
	shutdown(s,SHUT_WR);
	while (l){
		if (l<0){
			close(s);
			return SLL_RETURN_ERROR;
		}
		o->l+=l;
		o->v=realloc(o->v,o->l*sizeof(sll_char_t));
		memcpy(o->v+o->l-l-1,bf,l);
		l=recv(s,bf,4096,0);
	};
	o->l--;
	o->v[o->l]=0;
	for (sll_string_length_t i=0;i<o->l;i++){
		o->c^=o->v[i];
	}
	close(s);
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC void sll_platform_reset_console(void){
}

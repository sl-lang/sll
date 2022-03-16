#include <sll/_internal/common.h>
#include <sll/_internal/dispatcher.h>
#include <sll/common.h>
#include <sll/platform/file.h>
#include <sll/thread.h>
#include <sll/types.h>
#include <fcntl.h>
#include <poll.h>
#ifdef __SLL_BUILD_LINUX
#include <sys/eventfd.h>
#endif
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>



#define FROM_HANDLE(fd) ((int)ADDR(fd))
#define TO_HANDLE(fd) ((sll_file_descriptor_t)PTR(fd))



#ifdef __SLL_BUILD_DARWIN
static int _linux_pipe_write_end;
#endif



static __SLL_FORCE_INLINE void _platform_deinit_io_dispatcher(raw_event_data_t* r_dt,void* wait){
#ifdef __SLL_BUILD_DARWIN
	close(_linux_pipe_write_end);
#endif
	close(r_dt->fd);
	sem_close(wait);
	sem_unlink("/__sll_dispatcher_wait");
	sem_unlink("/__sll_dispatcher_notify");
}



static __SLL_FORCE_INLINE void _platform_init_io_dispatcher(raw_event_data_t* r_dt,void** wait){
#ifdef __SLL_BUILD_DARWIN
	int pipe_fd[2];
	SLL_CRITICAL(!pipe(pipe_fd));
	r_dt->fd=pipe_fd[0];
	_linux_pipe_write_end=pipe_fd[1];
#else
	r_dt->fd=eventfd(0,0);
#endif
	r_dt->events=POLLIN;
	*wait=sem_open("/__sll_dispatcher_wait",O_CREAT,S_IRUSR|S_IWUSR,0);
}



static __SLL_FORCE_INLINE event_list_length_t _platform_poll_events(raw_event_data_t* dt,void* wait,event_list_length_t cnt){
	int o=poll(dt,cnt,-1);
	if (o<1){
		return cnt;
	}
	o=0;
	while (o<cnt){
		if ((dt+o)->revents&POLLIN){
			break;
		}
		o++;
	}
	if (o==cnt){
		return cnt;
	}
	if (!o){
#ifdef __SLL_BUILD_DARWIN
		sll_char_t val;
		if (read(dt->fd,&val,sizeof(sll_char_t))!=sizeof(sll_char_t)){
#else
		__SLL_U64 val;
		if (read(dt->fd,&val,sizeof(__SLL_U64))!=sizeof(__SLL_U64)){
#endif
			SLL_UNIMPLEMENTED();
		}
		sem_post(wait);
		return 0;
	}
	return (sll_platform_file_data_available(TO_HANDLE((dt+o)->fd))?o:cnt);
}



static __SLL_FORCE_INLINE void _platform_poll_start(raw_event_data_t* dt){
#ifdef __SLL_BUILD_DARWIN
	sll_char_t val=1;
	if (write(_linux_pipe_write_end,&val,sizeof(sll_char_t))!=sizeof(sll_char_t)){
#else
	__SLL_U64 val=1;
	if (write(dt->fd,&val,sizeof(__SLL_U64))!=sizeof(__SLL_U64)){
#endif
		SLL_UNIMPLEMENTED();
	}
}



static __SLL_FORCE_INLINE void _platform_poll_stop(raw_event_data_t* dt,void** wait){
#ifdef __SLL_BUILD_DARWIN
	sll_char_t val=1;
	if (write(_linux_pipe_write_end,&val,sizeof(sll_char_t))!=sizeof(sll_char_t)){
#else
	__SLL_U64 val=1;
	if (write(dt->fd,&val,sizeof(__SLL_U64))!=sizeof(__SLL_U64)){
#endif
		SLL_UNIMPLEMENTED();
	}
	if (!wait){
		return;
	}
	sem_wait(*wait);
}



static __SLL_FORCE_INLINE void _platform_wait_for_dispatch(raw_event_data_t* dt){
#ifdef __SLL_BUILD_DARWIN
	sll_char_t val;
	if (read(dt->fd,&val,sizeof(sll_char_t))!=sizeof(sll_char_t)){
#else
	__SLL_U64 val;
	if (read(dt->fd,&val,sizeof(__SLL_U64))!=sizeof(__SLL_U64)){
#endif
		SLL_UNIMPLEMENTED();
	}
}

#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/file.h>
#include <sll/platform/socket.h>
#include <sll/socket.h>
#include <sll/types.h>
#ifdef __SLL_BUILD_WINDOWS
#include <winsock2.h>
#else
#include <sys/ioctl.h>
#endif



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_accept(sll_file_descriptor_t socket,sll_file_descriptor_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_bind(sll_file_descriptor_t socket,sll_address_t host,sll_port_t port){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_close(sll_file_descriptor_t socket){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_connect(sll_file_descriptor_t socket,sll_address_t host,sll_port_t port){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_create(sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_file_descriptor_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_socket_data_available(sll_file_descriptor_t socket){
#ifdef __SLL_BUILD_WINDOWS
	fd_set set;
	FD_ZERO(&set);
	FD_SET((int)ADDR(socket),&set);
	struct timeval timeout={
		0,
		0
	};
	return select(1,&set,NULL,NULL,&timeout)>0;
#else
	int count;
	ioctl((int)ADDR(socket),FIONREAD,&count);
	return !!count;
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_get_address_info(const sll_char_t* node,const sll_char_t* service,sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_address_info_flags_t flags,sll_address_info_t** out,sll_address_info_count_t* out_count){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_listen(sll_file_descriptor_t socket,sll_socket_queue_size_t queue_size){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_shutdown(sll_file_descriptor_t socket,sll_socket_shutdown_flags_t flags){
	SLL_UNIMPLEMENTED();
}

#include <sll/api/file.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/socket.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_accept(sll_file_handle_t socket,sll_file_handle_t* out){
	sll_file_t* socket_file=sll_file_from_handle(socket);
	if (!socket_file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_file_t connection;
	sll_error_t err=sll_socket_accept(socket_file,&connection);
	if (err==SLL_NO_ERROR){
		*out=sll_file_to_handle(&connection);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_bind(sll_file_handle_t socket,sll_address_t address,sll_port_t port){
	sll_file_t* socket_file=sll_file_from_handle(socket);
	return (!socket_file?SLL_ERROR_UNKNOWN_FD:sll_socket_bind(socket_file,address,port));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_connect(sll_file_handle_t socket,sll_address_t address,sll_port_t port){
	sll_file_t* socket_file=sll_file_from_handle(socket);
	return (!socket_file?SLL_ERROR_UNKNOWN_FD:sll_socket_connect(socket_file,address,port));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_create(sll_address_family_t address_family,sll_socket_type_t type,sll_file_handle_t* out){
	sll_file_t socket;
	sll_error_t err=sll_socket_create(address_family,type,&socket);
	if (err==SLL_NO_ERROR){
		*out=sll_file_to_handle(&socket);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_listen(sll_file_handle_t socket,sll_socket_queue_size_t queue_size){
	sll_file_t* socket_file=sll_file_from_handle(socket);
	return (!socket_file?SLL_ERROR_UNKNOWN_FD:sll_socket_listen(socket_file,queue_size));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_shutdown(sll_file_handle_t socket,sll_socket_shutdown_flags_t flags){
	sll_file_t* socket_file=sll_file_from_handle(socket);
	return (!socket_file?SLL_ERROR_UNKNOWN_FD:sll_socket_shutdown(socket_file,flags));
}

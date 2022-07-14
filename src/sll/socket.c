#include <sll/_internal/common.h>
#include <sll/_internal/file.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/platform/lock.h>
#include <sll/platform/socket.h>
#include <sll/socket.h>



__SLL_EXTERNAL sll_error_t sll_socket_accept(sll_file_t* socket,sll_file_t* out){
	if (!(socket->flags&SLL_FILE_FLAG_SOCKET)){
		return SLL_ERROR_NOT_A_SOCKET;
	}
	if (!(socket->flags&FILE_FLAG_PASSIVE)){
		SLL_UNIMPLEMENTED();
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL sll_error_t sll_socket_bind(sll_file_t* socket,sll_address_t host,sll_port_t port){
	if (!(socket->flags&SLL_FILE_FLAG_SOCKET)){
		return SLL_ERROR_NOT_A_SOCKET;
	}
	sll_error_t err=sll_platform_socket_bind(socket->data.socket.fd,host,port);
	if (err==SLL_NO_ERROR){
		(*((sll_file_flags_t*)(&(socket->flags))))|=FILE_FLAG_BOUND;
	}
	return err;
}



__SLL_EXTERNAL sll_error_t sll_socket_connect(sll_file_t* socket,sll_address_t host,sll_port_t port){
	if (!(socket->flags&SLL_FILE_FLAG_SOCKET)){
		return SLL_ERROR_NOT_A_SOCKET;
	}
	sll_error_t err=sll_platform_socket_connect(socket->data.socket.fd,host,port);
	if (err==SLL_NO_ERROR){
		(*((sll_file_flags_t*)(&(socket->flags))))|=SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE;
	}
	return err;
}



__SLL_EXTERNAL sll_error_t sll_socket_create(sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_file_t* out){
	sll_error_t err=sll_platform_socket_create(address_family,type,protocol,(sll_file_descriptor_t*)(&(out->data.socket.fd)));
	if (err!=SLL_NO_ERROR){
		return err;
	}
	*((sll_file_flags_t*)(&(out->flags)))=SLL_FILE_FLAG_ASYNC|SLL_FILE_FLAG_SOCKET;
	out->_lock=sll_platform_lock_create(NULL);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL sll_error_t sll_socket_listen(sll_file_t* socket,sll_socket_queue_size_t queue_size){
	if (!(socket->flags&SLL_FILE_FLAG_SOCKET)){
		return SLL_ERROR_NOT_A_SOCKET;
	}
	if (!(socket->flags&FILE_FLAG_BOUND)){
		SLL_UNIMPLEMENTED();
	}
	if (socket->flags&FILE_FLAG_PASSIVE){
		SLL_UNIMPLEMENTED();
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL sll_error_t sll_socket_shutdown(sll_file_t* socket,sll_socket_shutdown_flags_t flags){
	if (!(socket->flags&SLL_FILE_FLAG_SOCKET)){
		return SLL_ERROR_NOT_A_SOCKET;
	}
	SLL_UNIMPLEMENTED();
}

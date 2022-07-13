#include <sll/_internal/common.h>
#include <sll/api/file.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/socket.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_accept(sll_file_handle_t socket,sll_file_handle_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_bind(sll_file_handle_t socket,sll_address_t address,sll_port_t port){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_connect(sll_file_handle_t socket,sll_address_t address,sll_port_t port){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_create(sll_address_family_t address_family,sll_socket_type_t type,sll_file_handle_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_listen(sll_file_handle_t socket,sll_socket_queue_size_t queue_size){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_shutdown(sll_file_handle_t socket,sll_socket_shutdown_flags_t flags){
	SLL_UNIMPLEMENTED();
}

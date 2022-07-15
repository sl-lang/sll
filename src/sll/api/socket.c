#include <sll/_internal/common.h>
#include <sll/api/file.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/platform/socket.h>
#include <sll/socket.h>
#include <sll/static_object.h>
#include <sll/types.h>



static sll_object_t* _address_to_object(sll_address_t* address){
	switch (address->type){
		case SLL_ADDRESS_TYPE_UNKNOWN:
			return sll_array_to_object(NULL);
		case SLL_ADDRESS_TYPE_IPV4:
			return sll_new_object(SLL_CHAR("uu"),address->data.ipv4.address,address->data.ipv4.port);
		case SLL_ADDRESS_TYPE_IPV6:
			return sll_new_object(SLL_CHAR("[uuuuuuuu]uuu"),address->data.ipv6.address[0],address->data.ipv6.address[1],address->data.ipv6.address[2],address->data.ipv6.address[3],address->data.ipv6.address[4],address->data.ipv6.address[5],address->data.ipv6.address[6],address->data.ipv6.address[7],address->data.ipv6.flow_info,address->data.ipv6.scope_id,address->data.ipv6.port);
	}
	SLL_UNREACHABLE();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_accept(sll_file_handle_t socket,sll_file_handle_t* out){
	sll_file_t* socket_file=sll_file_from_handle(socket);
	if (!socket_file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_file_t connection;
	sll_error_t err=sll_socket_accept(socket_file,&connection);
	if (err==SLL_NO_ERROR){
		*out=sll_file_to_handle(&connection,1);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_bind(sll_file_handle_t socket,const sll_array_t* array){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_connect(sll_file_handle_t socket,const sll_array_t* array){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_create(sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_file_handle_t* out){
	sll_file_t socket;
	sll_error_t err=sll_socket_create(address_family,type,protocol,&socket);
	if (err==SLL_NO_ERROR){
		*out=sll_file_to_handle(&socket,1);
	}
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_get_address_info(const sll_string_t* node,const sll_string_t* service,sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_address_info_flags_t flags,sll_array_t* out){
	sll_address_info_t* data;
	sll_address_info_count_t count;
	sll_error_t err=sll_platform_socket_get_address_info(node->data,service->data,address_family,type,protocol,flags,&data,&count);
	if (err==SLL_NO_ERROR){
		sll_object_t* tmp=sll_new_object(SLL_CHAR("{MMu#}"),data,count,sizeof(sll_address_info_t),SLL_OFFSETOF(sll_address_info_t,address_family),0xffull,SLL_OFFSETOF(sll_address_info_t,type),0xffull,SLL_OFFSETOF(sll_address_info_t,protocol),_address_to_object,SLL_OFFSETOF(sll_address_info_t,address));
		*out=tmp->data.array;
		SLL_CRITICAL(sll_destroy_object(tmp));
	}
	sll_deallocate(data);
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
#include <sll/_internal/common.h>
#include <sll/_size_types.h>
#include <sll/api/file.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/platform/socket.h>
#include <sll/socket.h>
#include <sll/static_object.h>
#include <sll/types.h>



static sll_object_t _address_to_object(sll_address_t* address){
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



static sll_bool_t _object_to_address(const sll_array_t* data,sll_address_t* out){
	switch (data->length){
		case 0:
			out->type=SLL_ADDRESS_TYPE_UNKNOWN;
			return 1;
		case 2:
			if (data->data[0]->type!=SLL_OBJECT_TYPE_INT||data->data[0]->data.int_<0||data->data[0]->data.int_>__SLL_U32_MAX||data->data[1]->type!=SLL_OBJECT_TYPE_INT||data->data[1]->data.int_<0||data->data[1]->data.int_>__SLL_U16_MAX){
				return 0;
			}
			out->type=SLL_ADDRESS_TYPE_IPV4;
			out->data.ipv4.address=(__SLL_U32)(data->data[0]->data.int_);
			out->data.ipv4.port=(sll_port_t)(data->data[1]->data.int_);
			return 1;
		case 4:
			if (data->data[0]->type!=SLL_OBJECT_TYPE_ARRAY||data->data[0]->data.array.length!=8||data->data[1]->type!=SLL_OBJECT_TYPE_INT||data->data[1]->data.int_<0||data->data[1]->data.int_>__SLL_U32_MAX||data->data[2]->type!=SLL_OBJECT_TYPE_INT||data->data[2]->data.int_<0||data->data[2]->data.int_>__SLL_U32_MAX||data->data[3]->type!=SLL_OBJECT_TYPE_INT||data->data[3]->data.int_<0||data->data[3]->data.int_>__SLL_U16_MAX){
				return 0;
			}
			out->type=SLL_ADDRESS_TYPE_IPV6;
			for (sll_array_length_t i=0;i<8;i++){
				sll_object_t elem=data->data[0]->data.array.data[i];
				if (elem->type!=SLL_OBJECT_TYPE_INT||elem->data.int_<0||elem->data.int_>__SLL_U16_MAX){
					return 0;
				}
				out->data.ipv6.address[i]=(__SLL_U16)(elem->data.int_);
			}
			out->data.ipv6.flow_info=(__SLL_U32)(data->data[1]->data.int_);
			out->data.ipv6.scope_id=(__SLL_U32)(data->data[2]->data.int_);
			out->data.ipv6.port=(sll_port_t)(data->data[3]->data.int_);
			return 1;
	}
	return 0;
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
	sll_file_t* socket_file=sll_file_from_handle(socket);
	if (!socket_file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_address_t addr;
	if (!_object_to_address(array,&addr)){
		SLL_UNIMPLEMENTED();
	}
	return sll_socket_bind(socket_file,&addr);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_connect(sll_file_handle_t socket,const sll_array_t* array){
	sll_file_t* socket_file=sll_file_from_handle(socket);
	if (!socket_file){
		return SLL_ERROR_UNKNOWN_FD;
	}
	sll_address_t addr;
	if (!_object_to_address(array,&addr)){
		SLL_UNIMPLEMENTED();
	}
	return sll_socket_connect(socket_file,&addr);
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
		sll_object_t tmp=sll_new_object(SLL_CHAR("{MMu#}"),data,count,sizeof(sll_address_info_t),SLL_OFFSETOF(sll_address_info_t,address_family),0xffull,SLL_OFFSETOF(sll_address_info_t,type),0xffull,SLL_OFFSETOF(sll_address_info_t,protocol),_address_to_object,SLL_OFFSETOF(sll_address_info_t,address));
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

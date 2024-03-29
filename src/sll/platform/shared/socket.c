#ifdef __SLL_BUILD_WINDOWS
#include <ws2tcpip.h>
#include <winsock2.h>
#else
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/intrinsics.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/memory.h>
#include <sll/platform/file.h>
#include <sll/platform/socket.h>
#include <sll/platform/util.h>
#include <sll/socket.h>
#include <sll/types.h>



#ifdef __SLL_BUILD_WINDOWS
#define FROM_HANDLE(x) ((SOCKET)ADDR(x))
#else
#define FROM_HANDLE(x) ((int)ADDR(x))
#define SOCKET_ERROR -1
#endif
#define TO_HANDLE(x) (PTR(x))



#ifdef __SLL_BUILD_WINDOWS
typedef __SLL_S64 ssize_t;
#endif



static int _from_address_family(sll_socket_address_family_t address_family){
	switch (address_family){
		case 0:
			return 0;
		case SLL_SOCKET_ADDRESS_FAMILY_INET:
			return AF_INET;
		case SLL_SOCKET_ADDRESS_FAMILY_INET6:
			return AF_INET6;
	}
	SLL_UNIMPLEMENTED();
}



static sll_socket_address_family_t _to_address_family(int address_family){
	switch (address_family){
		case 0:
			return 0;
		case AF_INET:
			return SLL_SOCKET_ADDRESS_FAMILY_INET;
		case AF_INET6:
			return SLL_SOCKET_ADDRESS_FAMILY_INET6;
	}
	SLL_UNIMPLEMENTED();
}



static int _from_type(sll_socket_type_t type){
	switch (type){
		case 0:
			return 0;
		case SLL_SOCKET_TYPE_STREAM:
			return SOCK_STREAM;
		case SLL_SOCKET_TYPE_DGRAM:
			return SOCK_DGRAM;
		case SLL_SOCKET_TYPE_RAW:
			return SOCK_RAW;
		case SLL_SOCKET_TYPE_RDM:
			return SOCK_RDM;
		case SLL_SOCKET_TYPE_SEQPACKET:
			return SOCK_SEQPACKET;
	}
	SLL_UNIMPLEMENTED();
}



static sll_socket_type_t _to_type(int type){
	switch (type){
		case 0:
			return 0;
		case SOCK_STREAM:
			return SLL_SOCKET_TYPE_STREAM;
		case SOCK_DGRAM:
			return SLL_SOCKET_TYPE_DGRAM;
		case SOCK_RAW:
			return SLL_SOCKET_TYPE_RAW;
		case SOCK_RDM:
			return SLL_SOCKET_TYPE_RDM;
		case SOCK_SEQPACKET:
			return SLL_SOCKET_TYPE_SEQPACKET;
	}
	SLL_UNIMPLEMENTED();
}



static void _build_address(const struct sockaddr* addr,socklen_t addr_len,sll_address_t* out){
	out->type=SLL_ADDRESS_TYPE_UNKNOWN;
	switch (addr->sa_family){
		case AF_INET:
			{
				const struct sockaddr_in* ipv4_addr=(const struct sockaddr_in*)addr;
				out->type=SLL_ADDRESS_TYPE_IPV4;
				out->data.ipv4.address=SWAP_BYTES(*((__SLL_U32*)(&(ipv4_addr->sin_addr))));
				out->data.ipv4.port=SWAP_BYTES16(ipv4_addr->sin_port);
				return;
			}
		case AF_INET6:
			{
				const struct sockaddr_in6* ipv6_addr=(const struct sockaddr_in6*)addr;
				out->type=SLL_ADDRESS_TYPE_IPV6;
				const __SLL_U16* data=(__SLL_U16*)(&(ipv6_addr->sin6_addr));
				for (sll_array_length_t i=0;i<8;i++){
					out->data.ipv6.address[i]=SWAP_BYTES16(*(data+i));
				}
				out->data.ipv6.flow_info=ipv6_addr->sin6_flowinfo;
				out->data.ipv6.scope_id=ipv6_addr->sin6_scope_id;
				out->data.ipv6.port=SWAP_BYTES16(ipv6_addr->sin6_port);
				return;
			}
	}
	SLL_UNIMPLEMENTED();
}



static socklen_t _build_sockaddr(const sll_address_t* address,struct sockaddr** out){
	switch (address->type){
		case SLL_ADDRESS_TYPE_IPV4:
			{
				struct sockaddr_in* data=sll_allocate_stack(sizeof(struct sockaddr_in));
				data->sin_family=AF_INET;
				*((__SLL_U32*)(&(data->sin_addr)))=SWAP_BYTES(address->data.ipv4.address);
				data->sin_port=SWAP_BYTES16(address->data.ipv4.port);
				*out=(struct sockaddr*)data;
				return sizeof(struct sockaddr_in);
			}
		case SLL_ADDRESS_TYPE_IPV6:
			{
				struct sockaddr_in6* data=sll_allocate_stack(sizeof(struct sockaddr_in6));
				data->sin6_family=AF_INET6;
				__SLL_U16* address_data=(__SLL_U16*)(&(data->sin6_addr));
				for (sll_array_length_t i=0;i<8;i++){
					*(address_data+i)=SWAP_BYTES16(address->data.ipv6.address[i]);
				}
				data->sin6_flowinfo=address->data.ipv6.flow_info;
				data->sin6_scope_id=address->data.ipv6.scope_id;
				data->sin6_port=SWAP_BYTES16(address->data.ipv6.port);
				*out=(struct sockaddr*)data;
				return sizeof(struct sockaddr_in6);
			}
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_accept(sll_file_descriptor_t socket,sll_file_descriptor_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_bind(sll_file_descriptor_t socket,const sll_address_t* address){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_close(sll_file_descriptor_t socket){
#ifdef __SLL_BUILD_WINDOWS
	if (closesocket(FROM_HANDLE(socket))==SOCKET_ERROR){
		return sll_error_from_string_pointer(SLL_CHAR(gai_strerror(WSAGetLastError())));
	}
	return SLL_NO_ERROR;
#else
	return (close(FROM_HANDLE(socket))?sll_platform_get_error():SLL_NO_ERROR);
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_connect(sll_file_descriptor_t socket,const sll_address_t* address){
	struct sockaddr* addr;
	socklen_t addr_len=_build_sockaddr(address,&addr);
	int ret=connect(FROM_HANDLE(socket),addr,addr_len);
	sll_deallocate(addr);
	if (ret==SOCKET_ERROR){
		return sll_platform_get_error();
	}
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_create(sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_file_descriptor_t* out){
	sll_file_descriptor_t ret=(sll_file_descriptor_t)PTR(socket(_from_address_family(address_family),_from_type(type),protocol));
	if (ret==PTR(SOCKET_ERROR)){
		return sll_platform_get_error();
	}
	*out=ret;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_socket_data_available(sll_file_descriptor_t socket){
#ifdef __SLL_BUILD_WINDOWS
	fd_set set;
	FD_ZERO(&set);
	FD_SET(FROM_HANDLE(socket),&set);
	struct timeval timeout={
		0,
		0
	};
	return select(1,&set,NULL,NULL,&timeout)>0;
#else
	int count;
	ioctl(FROM_HANDLE(socket),FIONREAD,&count);
	return !!count;
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_get_address_info(const sll_char_t* node,const sll_char_t* service,sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_address_info_flags_t flags,sll_address_info_t** out,sll_address_info_count_t* out_count){
	*out=NULL;
	*out_count=0;
	struct addrinfo hints={
		flags,
		_from_address_family(address_family),
		_from_type(type),
		protocol,
		0,
		NULL,
		NULL,
		NULL
	};
	struct addrinfo* result;
	int err=getaddrinfo((const char*)node,(const char*)service,&hints,&result);
	if (err){
		return sll_error_from_string_pointer(SLL_CHAR(gai_strerror(err)));
	}
	sll_address_info_count_t i=0;
	for (struct addrinfo* ai=result;ai;ai=ai->ai_next){
		i++;
	}
	sll_address_info_t* data=sll_allocate_stack(i*sizeof(sll_address_info_t));
	*out=data;
	*out_count=i;
	for (struct addrinfo* ai=result;ai;ai=ai->ai_next){
		data->address_family=_to_address_family(ai->ai_family);
		data->type=_to_type(ai->ai_socktype);
		data->protocol=ai->ai_protocol;
		_build_address(ai->ai_addr,(socklen_t)(ai->ai_addrlen),&(data->address));
		data++;
	}
	freeaddrinfo(result);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_listen(sll_file_descriptor_t socket,sll_socket_queue_size_t queue_size){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_socket_read(sll_file_descriptor_t socket,void* pointer,sll_size_t size,sll_error_t* err){
	ERROR_PTR_RESET;
	ssize_t o=(ssize_t)recv(FROM_HANDLE(socket),pointer,(int)size,0);
	if (o==SOCKET_ERROR){
		ERROR_PTR_SYSTEM;
		return SLL_NO_FILE_SIZE;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_shutdown(sll_file_descriptor_t socket,sll_socket_shutdown_flags_t flags){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_platform_socket_write(sll_file_descriptor_t socket,const void* pointer,sll_size_t size,sll_error_t* err){
	ERROR_PTR_RESET;
	ssize_t o=(ssize_t)send(FROM_HANDLE(socket),pointer,(int)size,0);
	if (o==SOCKET_ERROR){
		ERROR_PTR_SYSTEM;
		return SLL_NO_FILE_SIZE;
	}
	return o;
}

#ifndef __SLL__SOCKET_TYPES_H__
#define __SLL__SOCKET_TYPES_H__ 1
#include <sll/_file_descriptor.h>
#include <sll/_size_types.h>



/**
 * \flags macro var
 * \name SLL_ADDRESS_TYPE_UNKNOWN
 * \group socket
 * \desc Docs!
 * \type sll_address_type_t
 */
#define SLL_ADDRESS_TYPE_UNKNOWN 0



/**
 * \flags macro var
 * \name SLL_ADDRESS_TYPE_IPV4
 * \group socket
 * \desc Docs!
 * \type sll_address_type_t
 */
#define SLL_ADDRESS_TYPE_IPV4 1



/**
 * \flags macro var
 * \name SLL_ADDRESS_TYPE_IPV6
 * \group socket
 * \desc Docs!
 * \type sll_address_type_t
 */
#define SLL_ADDRESS_TYPE_IPV6 2



/**
 * \flags type var
 * \name sll_socket_address_family_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_socket_address_family_t;



/**
 * \flags type var
 * \name sll_socket_type_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_socket_type_t;



/**
 * \flags type var
 * \name sll_socket_protocol_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_socket_protocol_t;



/**
 * \flags type var
 * \name sll_port_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_port_t;



/**
 * \flags type var
 * \name sll_socket_queue_size_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_socket_queue_size_t;



/**
 * \flags type var
 * \name sll_socket_shutdown_flags_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_socket_shutdown_flags_t;



/**
 * \flags type var
 * \name sll_address_type_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_address_type_t;



/**
 * \flags type
 * \name sll_address_ipv4_t
 * \group socket
 * \desc Docs!
 * \arg __SLL_U32 address
 * \arg sll_port_t port
 */
typedef struct _SLL_ADDRESS_IPV4{
	__SLL_U32 address;
	sll_port_t port;
} sll_address_ipv4_t;



/**
 * \flags type
 * \name sll_address_ipv6_t
 * \group socket
 * \desc Docs!
 * \arg __SLL_U16 address[8]
 * \arg __SLL_U32 flow_info
 * \arg __SLL_U32 scope_id
 * \arg sll_port_t port
 */
typedef struct _SLL_ADDRESS_IPV6{
	__SLL_U16 address[8];
	__SLL_U32 flow_info;
	__SLL_U32 scope_id;
	sll_port_t port;
} sll_address_ipv6_t;



/**
 * \flags type union
 * \name sll_address_data_t
 * \group socket
 * \desc Docs!
 * \arg sll_address_ipv4_t ipv4
 * \arg sll_address_ipv6_t ipv6
 */
typedef union _SLL_ADDRESS_DATA{
	sll_address_ipv4_t ipv4;
	sll_address_ipv6_t ipv6;
} sll_address_data_t;



/**
 * \flags type
 * \name sll_address_t
 * \group socket
 * \desc Docs!
 * \arg sll_address_type_t type
 * \arg sll_address_data_t data
 */
typedef struct _SLL_ADDRESS{
	sll_address_type_t type;
	sll_address_data_t data;
} sll_address_t;



/**
 * \flags type
 * \name sll_file_data_socket_t
 * \group file
 * \desc Docs!
 * \arg const sll_file_descriptor_t fd
 */
typedef struct _SLL_FILE_DATA_SOCKET{
	const sll_file_descriptor_t fd;
} sll_file_data_socket_t;



#endif

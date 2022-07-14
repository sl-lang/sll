#ifndef __SLL__SOCKET_TYPES_H__
#define __SLL__SOCKET_TYPES_H__ 1
#include <sll/_file_descriptor.h>
#include <sll/_size_types.h>



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
 * \name sll_address_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_address_t;



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
 * \flags type
 * \name sll_file_data_socket_t
 * \group file
 * \desc Docs!
 * \arg const sll_file_descriptor_t fd
 * \arg const sll_socket_address_family_t address_family
 * \arg const sll_socket_type_t type
 * \arg const sll_socket_protocol_t protocol
 * \arg sll_address_t address
 * \arg sll_port_t port
 * \arg sll_socket_queue_size_t queue_size
 * \arg sll_socket_shutdown_flags_t shutdown_state
 */
typedef struct _SLL_FILE_DATA_SOCKET{
	const sll_file_descriptor_t fd;
	const sll_socket_address_family_t address_family;
	const sll_socket_type_t type;
	const sll_socket_protocol_t protocol;
	sll_address_t address;
	sll_port_t port;
	sll_socket_queue_size_t queue_size;
	sll_socket_shutdown_flags_t shutdown_state;
} sll_file_data_socket_t;



#endif

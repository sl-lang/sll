#ifndef __SLL_PLATFORM_SOCKET_H__
#define __SLL_PLATFORM_SOCKET_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/file.h>
#include <sll/socket.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Sockets
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_address_info_flags_t
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_address_info_flags_t;



/**
 * \flags type var
 * \name sll_address_info_count_t
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_address_info_count_t;



/**
 * \flags type
 * \name sll_address_info_t
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_socket_address_family_t address_family
 * \arg sll_socket_type_t type
 * \arg sll_socket_protocol_t protocol
 * \arg sll_address_t address
 */
typedef struct _SLL_ADDRESS_INFO{
	sll_socket_address_family_t address_family;
	sll_socket_type_t type;
	sll_socket_protocol_t protocol;
	sll_address_t address;
} sll_address_info_t;



/**
 * \flags check_output func
 * \name sll_platform_socket_accept
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_file_descriptor_t socket
 * \arg sll_file_descriptor_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_accept(sll_file_descriptor_t socket,sll_file_descriptor_t* out);



/**
 * \flags check_output func
 * \name sll_platform_socket_bind
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_file_descriptor_t socket
 * \arg sll_address_t host
 * \arg sll_port_t port
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_bind(sll_file_descriptor_t socket,sll_address_t host,sll_port_t port);



/**
 * \flags check_output func
 * \name sll_platform_socket_close
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_file_descriptor_t socket
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_close(sll_file_descriptor_t socket);



/**
 * \flags check_output func
 * \name sll_platform_socket_connect
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_file_descriptor_t socket
 * \arg sll_address_t host
 * \arg sll_port_t port
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_connect(sll_file_descriptor_t socket,sll_address_t host,sll_port_t port);



/**
 * \flags check_output func
 * \name sll_platform_socket_create
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_socket_address_family_t address_family
 * \arg sll_socket_type_t type
 * \arg sll_socket_protocol_t protocol
 * \arg sll_file_descriptor_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_create(sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_file_descriptor_t* out);



/**
 * \flags check_output func
 * \name sll_platform_socket_data_available
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_file_descriptor_t socket
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_socket_data_available(sll_file_descriptor_t socket);



/**
 * \flags check_output func
 * \name sll_platform_socket_get_address_info
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg const sll_char_t* node
 * \arg const sll_char_t* service
 * \arg sll_socket_address_family_t address_family
 * \arg sll_socket_type_t type
 * \arg sll_socket_protocol_t protocol
 * \arg sll_address_info_flags_t flags
 * \arg sll_address_info_t** out
 * \arg sll_address_info_count_t* out_count
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_get_address_info(const sll_char_t* node,const sll_char_t* service,sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_address_info_flags_t flags,sll_address_info_t** out,sll_address_info_count_t* out_count);



/**
 * \flags check_output func
 * \name sll_platform_socket_listen
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_file_descriptor_t socket
 * \arg sll_socket_queue_size_t queue_size
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_listen(sll_file_descriptor_t socket,sll_socket_queue_size_t queue_size);



/**
 * \flags check_output func
 * \name sll_platform_socket_shutdown
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg sll_file_descriptor_t socket
 * \arg sll_socket_shutdown_flags_t flags
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_shutdown(sll_file_descriptor_t socket,sll_socket_shutdown_flags_t flags);



#endif

#ifndef __SLL_PLATFORM_SOCKET_H__
#define __SLL_PLATFORM_SOCKET_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/error.h>
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
 * \arg sll_socket_ddress_family_t address_family
 * \arg sll_socket_type_t type
 * \arg sll_socket_protocol_t protocol
 * \arg sll_address_t address
 */
typedef struct _SLL_ADDRESS_INFO{
	sll_socket_ddress_family_t address_family;
	sll_socket_type_t type;
	sll_socket_protocol_t protocol;
	sll_address_t address;
} sll_address_info_t;



/**
 * \flags check_output func
 * \name sll_platform_socket_get_address_info
 * \group platform
 * \subgroup platform-socket
 * \desc Docs!
 * \arg const sll_char_t* node
 * \arg const sll_char_t* service
 * \arg sll_socket_ddress_family_t address_family
 * \arg sll_socket_type_t type
 * \arg sll_socket_protocol_t protocol
 * \arg sll_address_info_flags_t flags
 * \arg sll_address_info_t** out
 * \arg sll_address_info_count_t* out_count
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_socket_get_address_info(const sll_char_t* node,const sll_char_t* service,sll_socket_ddress_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_address_info_flags_t flags,sll_address_info_t** out,sll_address_info_count_t* out_count);



#endif

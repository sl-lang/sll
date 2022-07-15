#ifndef __SLL_SOCKET_H__
#define __SLL_SOCKET_H__ 1
#include <sll/_size_types.h>
#include <sll/_socket_types.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
/**
 * \flags group
 * \name Sockets
 * \group socket
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_SOCKET_ADDRESS_FAMILY_INET
 * \group socket
 * \desc Docs!
 * \type sll_socket_address_family_t
 */
#define SLL_SOCKET_ADDRESS_FAMILY_INET 1



/**
 * \flags macro var
 * \name SLL_SOCKET_ADDRESS_FAMILY_INET6
 * \group socket
 * \desc Docs!
 * \type sll_socket_address_family_t
 */
#define SLL_SOCKET_ADDRESS_FAMILY_INET6 2



/**
 * \flags macro var
 * \name SLL_SOCKET_TYPE_STREAM
 * \group socket
 * \desc Docs!
 * \type sll_socket_type_t
 */
#define SLL_SOCKET_TYPE_STREAM 1



/**
 * \flags macro var
 * \name SLL_SOCKET_TYPE_DGRAM
 * \group socket
 * \desc Docs!
 * \type sll_socket_type_t
 */
#define SLL_SOCKET_TYPE_DGRAM 2



/**
 * \flags macro var
 * \name SLL_SOCKET_TYPE_RAW
 * \group socket
 * \desc Docs!
 * \type sll_socket_type_t
 */
#define SLL_SOCKET_TYPE_RAW 3



/**
 * \flags macro var
 * \name SLL_SOCKET_TYPE_RDM
 * \group socket
 * \desc Docs!
 * \type sll_socket_type_t
 */
#define SLL_SOCKET_TYPE_RDM 4



/**
 * \flags macro var
 * \name SLL_SOCKET_TYPE_SEQPACKET
 * \group socket
 * \desc Docs!
 * \type sll_socket_type_t
 */
#define SLL_SOCKET_TYPE_SEQPACKET 5



/**
 * \flags macro var
 * \name SLL_SOCKET_SHUTDOWN_FLAG_READ
 * \group socket
 * \desc Docs!
 * \type sll_socket_shutdown_flags_t
 */
#define SLL_SOCKET_SHUTDOWN_FLAG_READ 1



/**
 * \flags macro var
 * \name SLL_SOCKET_SHUTDOWN_FLAG_WRITE
 * \group socket
 * \desc Docs!
 * \type sll_socket_shutdown_flags_t
 */
#define SLL_SOCKET_SHUTDOWN_FLAG_WRITE 2



/**
 * \flags check_output func
 * \name sll_socket_accept
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_file_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_socket_accept(sll_file_t* socket,sll_file_t* out);



/**
 * \flags check_output func
 * \name sll_socket_bind
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg const sll_address_t* address
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_socket_bind(sll_file_t* socket,const sll_address_t* address);



/**
 * \flags check_output func
 * \name sll_socket_connect
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg const sll_address_t* address
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_socket_connect(sll_file_t* socket,const sll_address_t* address);



/**
 * \flags check_output func
 * \name sll_socket_create
 * \group socket
 * \desc Docs!
 * \arg sll_socket_address_family_t address_family
 * \arg sll_socket_type_t type
 * \arg sll_socket_protocol_t protocol
 * \arg sll_file_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_socket_create(sll_socket_address_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_file_t* out);



/**
 * \flags check_output func
 * \name sll_socket_listen
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_socket_queue_size_t queue_size
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_socket_listen(sll_file_t* socket,sll_socket_queue_size_t queue_size);



/**
 * \flags check_output func
 * \name sll_socket_shutdown
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_socket_shutdown_flags_t flags
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_socket_shutdown(sll_file_t* socket,sll_socket_shutdown_flags_t flags);



#endif

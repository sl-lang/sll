#ifndef __SLL_SOCKET_H__
#define __SLL_SOCKET_H__ 1
#include <sll/_size_types.h>
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
 * \type sll_socket_ddress_family_t
 */
#define SLL_SOCKET_ADDRESS_FAMILY_INET 0



/**
 * \flags macro var
 * \name SLL_SOCKET_ADDRESS_FAMILY_INET6
 * \group socket
 * \desc Docs!
 * \type sll_socket_ddress_family_t
 */
#define SLL_SOCKET_ADDRESS_FAMILY_INET6 1



/**
 * \flags macro var
 * \name SLL_SOCKET_TYPE_STREAM
 * \group socket
 * \desc Docs!
 * \type sll_socket_type_t
 */
#define SLL_SOCKET_TYPE_STREAM 0



/**
 * \flags macro var
 * \name SLL_SOCKET_TYPE_DATAGRAM
 * \group socket
 * \desc Docs!
 * \type sll_socket_type_t
 */
#define SLL_SOCKET_TYPE_DATAGRAM 1



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
 * \flags type var
 * \name sll_socket_ddress_family_t
 * \group socket
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_socket_ddress_family_t;



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
 * \flags func
 * \name sll_socket_accept
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_file_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_socket_accept(sll_file_t* socket,sll_file_t* out);



/**
 * \flags func
 * \name sll_socket_bind
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_address_t host
 * \arg sll_port_t port
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_socket_bind(sll_file_t* socket,sll_address_t host,sll_port_t port);



/**
 * \flags func
 * \name sll_socket_connect
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_address_t host
 * \arg sll_port_t port
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_socket_connect(sll_file_t* socket,sll_address_t host,sll_port_t port);



/**
 * \flags func
 * \name sll_socket_create
 * \group socket
 * \desc Docs!
 * \arg sll_socket_ddress_family_t address_family
 * \arg sll_socket_type_t type
 * \arg sll_socket_protocol_t protocol
 * \arg sll_file_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_socket_create(sll_socket_ddress_family_t address_family,sll_socket_type_t type,sll_socket_protocol_t protocol,sll_file_t* out);



/**
 * \flags func
 * \name sll_socket_listen
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_socket_queue_size_t queue_size
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_socket_listen(sll_file_t* socket,sll_socket_queue_size_t queue_size);



/**
 * \flags func
 * \name sll_socket_shutdown
 * \group socket
 * \desc Docs!
 * \arg sll_file_t* socket
 * \arg sll_socket_shutdown_flags_t flags
 * \ret sll_error_t
 */
__SLL_EXTERNAL sll_error_t sll_socket_shutdown(sll_file_t* socket,sll_socket_shutdown_flags_t flags);



#endif

#ifndef __SLL_API_SOCKET_H__
#define __SLL_API_SOCKET_H__ 1
#include <sll/api/file.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/socket.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Socket API
 * \group socket-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_api_socket_accept
 * \group socket-api
 * \desc Docs!
 * \api D|~D
 * \arg sll_file_handle_t socket
 * \arg sll_file_handle_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_accept(sll_file_handle_t socket,sll_file_handle_t* out);



/**
 * \flags check_output func
 * \name sll_api_socket_bind
 * \group socket-api
 * \desc Docs!
 * \api DDW|Q
 * \arg sll_file_handle_t socket
 * \arg sll_address_t address
 * \arg sll_port_t port
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_socket_bind(sll_file_handle_t socket,sll_address_t address,sll_port_t port);



#endif

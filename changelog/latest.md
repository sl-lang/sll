# Sll [0.7.26] - Ongoing

## Added

- Ability to change window icons: `window_window$set_icon`
- Funcion to retrive the platform-specific window context: `window_util$get_context`
- Implemented the socket API: `SLL_SOCKET_ADDRESS_FAMILY_INET`, `SLL_SOCKET_ADDRESS_FAMILY_INET6`, `SLL_SOCKET_TYPE_STREAM`, `SLL_SOCKET_TYPE_DATAGRAM`, `SLL_SOCKET_SHUTDOWN_FLAG_READ`, `SLL_SOCKET_SHUTDOWN_FLAG_WRITE`, `sll_address_family_t`, `sll_socket_type_t`, `sll_address_t`, `sll_port_t`, `sll_socket_queue_size_t`, `sll_socket_shutdown_flags_t`, `sll_socket_accept`, `sll_socket_bind`, `sll_socket_connect`, `sll_socket_create`, `sll_socket_listen` and `sll_socket_shutdown`
- Map containers: `SLL_MAP_CONTAINER_FLAG_XOR`, `SLL_MAP_CONTAINER_HASH_XOR`, `sll_map_container_hash_callback_t`, `sll_map_container_equal_t`, `sll_map_container_filter_callback_t`, `sll_map_container_t`, `sll_map_container_clear`, `sll_map_container_contains`, `sll_map_container_deinit`, `sll_map_container_delete`, `sll_map_container_filter`, `sll_map_container_get`, `sll_map_container_init`, `sll_map_container_iter`, `sll_map_container_iter_clear` and `sll_map_container_set`
- New error: `SLL_ERROR_NOT_A_SOCKET`

## Changed

- Fixed variable deletion after infinite loops
- Improved performance of API calls

## Removed

- Obsolete TLS API

[0.7.26]: https://github.com/sl-lang/sll/compare/sll-v0.7.25...main

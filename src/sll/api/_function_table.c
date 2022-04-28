#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>



extern void sll_api_int_to_bin(void);
extern void sll_api_int_to_dec(void);
extern void sll_api_int_to_hex(void);
extern void sll_api_int_to_oct(void);
extern void sll_api_atexit_register(void);
extern void sll_api_atexit_unregister(void);
extern void sll_api_process_execute_shell(void);
extern void sll_api_process_get_pid(void);
extern void sll_api_process_join(void);
extern void sll_api_process_split(void);
extern void sll_api_process_start(void);
extern void sll_api_weakref__init(void);
extern void sll_api_weakref_create(void);
extern void sll_api_weakref_delete(void);
extern void sll_api_weakref_get(void);
extern void sll_api_weakref_set_callback_data(void);
extern void sll_api_error_get_call_stack(void);
extern void sll_api_log_log(void);
extern void sll_api_log_set_default(void);
extern void sll_api_log_set_file(void);
extern void sll_api_log_set_function(void);
extern void sll_api_sys_get_args(void);
extern void sll_api_sys_get_cpu_count(void);
extern void sll_api_sys_get_env(void);
extern void sll_api_sys_get_executable(void);
extern void sll_api_sys_get_library(void);
extern void sll_api_sys_get_platform(void);
extern void sll_api_sys_get_sandbox_flags(void);
extern void sll_api_sys_get_version(void);
extern void sll_api_sys_load_library(void);
extern void sll_api_sys_remove_env(void);
extern void sll_api_sys_set_sandbox_flag(void);
extern void sll_api_sys_set_env(void);
extern void sll_api_audit__init(void);
extern void sll_api_audit_audit(void);
extern void sll_api_object_new(void);
extern void sll_api_time_current(void);
extern void sll_api_time_current_ns(void);
extern void sll_api_time_sleep(void);
extern void sll_api_time_sleep_ns(void);
extern void sll_api_string_checksum(void);
extern void sll_api_string_convert(void);
extern void sll_api_string_count(void);
extern void sll_api_string_count_left(void);
extern void sll_api_string_count_right(void);
extern void sll_api_string_ends(void);
extern void sll_api_string_format(void);
extern void sll_api_string_flip_case(void);
extern void sll_api_string_index(void);
extern void sll_api_string_index_list(void);
extern void sll_api_string_index_reverse(void);
extern void sll_api_string_index_reverse_list(void);
extern void sll_api_string_join(void);
extern void sll_api_string_pad(void);
extern void sll_api_string_pad_left(void);
extern void sll_api_string_pad_right(void);
extern void sll_api_string_replace(void);
extern void sll_api_string_reverse(void);
extern void sll_api_string_secure_equal(void);
extern void sll_api_string_split(void);
extern void sll_api_string_starts(void);
extern void sll_api_string_to_lower_case(void);
extern void sll_api_string_to_title_case(void);
extern void sll_api_string_trim(void);
extern void sll_api_string_trim_left(void);
extern void sll_api_string_trim_right(void);
extern void sll_api_string_to_upper_case(void);
extern void sll_api_file_close(void);
extern void sll_api_file_copy(void);
extern void sll_api_file_delete(void);
extern void sll_api_file_flush(void);
extern void sll_api_file_from_data(void);
extern void sll_api_file_get_buffer(void);
extern void sll_api_file_get_temp_path(void);
extern void sll_api_file_inc_handle(void);
extern void sll_api_file_open(void);
extern void sll_api_file_peek(void);
extern void sll_api_file_read(void);
extern void sll_api_file_read_char(void);
extern void sll_api_file_rename(void);
extern void sll_api_file_std_handle(void);
extern void sll_api_file_write(void);
extern void sll_api_serial_decode_float(void);
extern void sll_api_serial_decode_integer(void);
extern void sll_api_serial_decode_signed_integer(void);
extern void sll_api_serial_decode_object(void);
extern void sll_api_serial_decode_string(void);
extern void sll_api_serial_encode_float(void);
extern void sll_api_serial_encode_integer(void);
extern void sll_api_serial_encode_signed_integer(void);
extern void sll_api_serial_encode_object(void);
extern void sll_api_serial_encode_string(void);
extern void sll_api_hash_md5(void);
extern void sll_api_hash_sha1(void);
extern void sll_api_hash_sha256(void);
extern void sll_api_hash_sha512(void);
extern void sll_api_vm_get_config(void);
extern void sll_api_vm_get_instruction_count(void);
extern void sll_api_vm_get_instruction_index(void);
extern void sll_api_vm_get_location(void);
extern void sll_api_vm_get_ref_count(void);
extern void sll_api_struct_float_from_bits(void);
extern void sll_api_struct_double_from_bits(void);
extern void sll_api_struct_float_to_bits(void);
extern void sll_api_struct_double_to_bits(void);
extern void sll_api_math_abs(void);
extern void sll_api_math_acos(void);
extern void sll_api_math_acosh(void);
extern void sll_api_math_asin(void);
extern void sll_api_math_asinh(void);
extern void sll_api_math_atan(void);
extern void sll_api_math_atan2(void);
extern void sll_api_math_atanh(void);
extern void sll_api_math_cbrt(void);
extern void sll_api_math_combinations(void);
extern void sll_api_math_cos(void);
extern void sll_api_math_cosh(void);
extern void sll_api_math_ceil(void);
extern void sll_api_math_copy_sign(void);
extern void sll_api_math_euler_phi(void);
extern void sll_api_math_exp(void);
extern void sll_api_math_factorial(void);
extern void sll_api_math_factors(void);
extern void sll_api_math_gcd(void);
extern void sll_api_math_floor(void);
extern void sll_api_math_int_log2(void);
extern void sll_api_math_int_pow(void);
extern void sll_api_math_int_sqrt(void);
extern void sll_api_math_log(void);
extern void sll_api_math_log2(void);
extern void sll_api_math_log10(void);
extern void sll_api_math_permutations(void);
extern void sll_api_math_pow(void);
extern void sll_api_math_round(void);
extern void sll_api_math_sin(void);
extern void sll_api_math_sinh(void);
extern void sll_api_math_sqrt(void);
extern void sll_api_math_tan(void);
extern void sll_api_math_tanh(void);
extern void sll_api_path_absolute(void);
extern void sll_api_path_exists(void);
extern void sll_api_path_get_cwd(void);
extern void sll_api_path_is_dir(void);
extern void sll_api_path_join(void);
extern void sll_api_path_list_dir(void);
extern void sll_api_path_mkdir(void);
extern void sll_api_path_relative(void);
extern void sll_api_path_set_cwd(void);
extern void sll_api_path_size(void);
extern void sll_api_path_split(void);
extern void sll_api_path_split_drive(void);
extern void sll_api_base64_decode(void);
extern void sll_api_base64_encode(void);
extern void sll_api_random_get_float(void);
extern void sll_api_random_get_int(void);
extern void sll_api_random_get_string(void);
extern void sll_api_map_extend(void);
extern void sll_api_map_remove(void);
extern void sll_api_date_get_time_zone(void);
extern void sll_api_date_merge(void);
extern void sll_api_date_split(void);
extern void sll_api_array_count(void);
extern void sll_api_array_create(void);
extern void sll_api_array_extend(void);
extern void sll_api_array_index(void);
extern void sll_api_array_join(void);
extern void sll_api_array_pop(void);
extern void sll_api_array_push(void);
extern void sll_api_array_remove(void);
extern void sll_api_array_replace(void);
extern void sll_api_array_reverse(void);
extern void sll_api_array_shift(void);
extern void sll_api_array_split(void);
extern void sll_api_array_unshift(void);
extern void sll_api_json__init(void);
extern void sll_api_json_parse(void);
extern void sll_api_json_stringify(void);
extern void sll_api_float_get_compare_error(void);
extern void sll_api_float_set_compare_error(void);
extern void sll_api_thread_create(void);
extern void sll_api_thread_create_barrier(void);
extern void sll_api_thread_create_lock(void);
extern void sll_api_thread_create_semaphore(void);
extern void sll_api_thread_delete(void);
extern void sll_api_thread_delete_barrier(void);
extern void sll_api_thread_delete_lock(void);
extern void sll_api_thread_delete_semaphore(void);
extern void sll_api_thread_get_internal_data(void);
extern void sll_api_thread_increase_barrier(void);
extern void sll_api_thread_release_lock(void);
extern void sll_api_thread_release_semaphore(void);
extern void sll_api_thread_reset_barrier(void);
extern void sll_api_thread_restart(void);
extern void sll_api_thread_suspend(void);
extern void sll_api_sort_quicksort(void);



static const sll_internal_function_descriptor_t _ifunc_data_ptr[]={
	{
		SLL_CHAR("sll:array_count"),
		sll_api_array_count,
		SLL_CHAR("#ao|D")
	},
	{
		SLL_CHAR("sll:array_create"),
		sll_api_array_create,
		SLL_CHAR("D|a")
	},
	{
		SLL_CHAR("sll:array_extend"),
		sll_api_array_extend,
		SLL_CHAR("a#a|D")
	},
	{
		SLL_CHAR("sll:array_index"),
		sll_api_array_index,
		SLL_CHAR("#ao|i")
	},
	{
		SLL_CHAR("sll:array_join"),
		sll_api_array_join,
		SLL_CHAR("#a+o|a")
	},
	{
		SLL_CHAR("sll:array_pop"),
		sll_api_array_pop,
		SLL_CHAR("a|o")
	},
	{
		SLL_CHAR("sll:array_push"),
		sll_api_array_push,
		SLL_CHAR("ao|D")
	},
	{
		SLL_CHAR("sll:array_remove"),
		sll_api_array_remove,
		SLL_CHAR("ao|D")
	},
	{
		SLL_CHAR("sll:array_replace"),
		sll_api_array_replace,
		SLL_CHAR("#aoo|a")
	},
	{
		SLL_CHAR("sll:array_reverse"),
		sll_api_array_reverse,
		SLL_CHAR("#a|a")
	},
	{
		SLL_CHAR("sll:array_shift"),
		sll_api_array_shift,
		SLL_CHAR("a|o")
	},
	{
		SLL_CHAR("sll:array_split"),
		sll_api_array_split,
		SLL_CHAR("#ao|a")
	},
	{
		SLL_CHAR("sll:array_unshift"),
		sll_api_array_unshift,
		SLL_CHAR("ao|D")
	},
	{
		SLL_CHAR("sll:atexit_register"),
		sll_api_atexit_register,
		SLL_CHAR("io!|")
	},
	{
		SLL_CHAR("sll:atexit_unregister"),
		sll_api_atexit_unregister,
		SLL_CHAR("i|b")
	},
	{
		SLL_CHAR("sll:audit__init"),
		sll_api_audit__init,
		SLL_CHAR("i|")
	},
	{
		SLL_CHAR("sll:audit_audit"),
		sll_api_audit_audit,
		SLL_CHAR("#s#so!|")
	},
	{
		SLL_CHAR("sll:base64_decode"),
		sll_api_base64_decode,
		SLL_CHAR("#s|~s")
	},
	{
		SLL_CHAR("sll:base64_encode"),
		sll_api_base64_encode,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:date_get_time_zone"),
		sll_api_date_get_time_zone,
		SLL_CHAR("|a")
	},
	{
		SLL_CHAR("sll:date_merge"),
		sll_api_date_merge,
		SLL_CHAR("WBBBBf|f")
	},
	{
		SLL_CHAR("sll:date_split"),
		sll_api_date_split,
		SLL_CHAR("f|a")
	},
	{
		SLL_CHAR("sll:error_get_call_stack"),
		sll_api_error_get_call_stack,
		SLL_CHAR("Di|a")
	},
	{
		SLL_CHAR("sll:file_close"),
		sll_api_file_close,
		SLL_CHAR("i|b")
	},
	{
		SLL_CHAR("sll:file_copy"),
		sll_api_file_copy,
		SLL_CHAR("#s#s|i")
	},
	{
		SLL_CHAR("sll:file_delete"),
		sll_api_file_delete,
		SLL_CHAR("#s|i")
	},
	{
		SLL_CHAR("sll:file_flush"),
		sll_api_file_flush,
		SLL_CHAR("i|b")
	},
	{
		SLL_CHAR("sll:file_from_data"),
		sll_api_file_from_data,
		SLL_CHAR("#sB|i")
	},
	{
		SLL_CHAR("sll:file_get_buffer"),
		sll_api_file_get_buffer,
		SLL_CHAR("i|s")
	},
	{
		SLL_CHAR("sll:file_get_temp_path"),
		sll_api_file_get_temp_path,
		SLL_CHAR("|s")
	},
	{
		SLL_CHAR("sll:file_inc_handle"),
		sll_api_file_inc_handle,
		SLL_CHAR("i|")
	},
	{
		SLL_CHAR("sll:file_open"),
		sll_api_file_open,
		SLL_CHAR("#sB|i")
	},
	{
		SLL_CHAR("sll:file_peek"),
		sll_api_file_peek,
		SLL_CHAR("i|~c")
	},
	{
		SLL_CHAR("sll:file_read"),
		sll_api_file_read,
		SLL_CHAR("iD|~s")
	},
	{
		SLL_CHAR("sll:file_read_char"),
		sll_api_file_read_char,
		SLL_CHAR("i|~c")
	},
	{
		SLL_CHAR("sll:file_rename"),
		sll_api_file_rename,
		SLL_CHAR("#s#s|i")
	},
	{
		SLL_CHAR("sll:file_std_handle"),
		sll_api_file_std_handle,
		SLL_CHAR("c|i")
	},
	{
		SLL_CHAR("sll:file_write"),
		sll_api_file_write,
		SLL_CHAR("i#s|i")
	},
	{
		SLL_CHAR("sll:float_get_compare_error"),
		sll_api_float_get_compare_error,
		SLL_CHAR("|f")
	},
	{
		SLL_CHAR("sll:float_set_compare_error"),
		sll_api_float_set_compare_error,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:hash_md5"),
		sll_api_hash_md5,
		SLL_CHAR("DDDD#s|a")
	},
	{
		SLL_CHAR("sll:hash_sha1"),
		sll_api_hash_sha1,
		SLL_CHAR("DDDDD#s|a")
	},
	{
		SLL_CHAR("sll:hash_sha256"),
		sll_api_hash_sha256,
		SLL_CHAR("DDDDDDDD#s|a")
	},
	{
		SLL_CHAR("sll:hash_sha512"),
		sll_api_hash_sha512,
		SLL_CHAR("QQQQQQQQ#s|a")
	},
	{
		SLL_CHAR("sll:int_to_bin"),
		sll_api_int_to_bin,
		SLL_CHAR("i|s")
	},
	{
		SLL_CHAR("sll:int_to_dec"),
		sll_api_int_to_dec,
		SLL_CHAR("i|s")
	},
	{
		SLL_CHAR("sll:int_to_hex"),
		sll_api_int_to_hex,
		SLL_CHAR("i|s")
	},
	{
		SLL_CHAR("sll:int_to_oct"),
		sll_api_int_to_oct,
		SLL_CHAR("i|s")
	},
	{
		SLL_CHAR("sll:json__init"),
		sll_api_json__init,
		SLL_CHAR("ooo|")
	},
	{
		SLL_CHAR("sll:json_parse"),
		sll_api_json_parse,
		SLL_CHAR("#s|o")
	},
	{
		SLL_CHAR("sll:json_stringify"),
		sll_api_json_stringify,
		SLL_CHAR("o|s")
	},
	{
		SLL_CHAR("sll:log_log"),
		sll_api_log_log,
		SLL_CHAR("o!|b")
	},
	{
		SLL_CHAR("sll:log_set_default"),
		sll_api_log_set_default,
		SLL_CHAR("Bb|b")
	},
	{
		SLL_CHAR("sll:log_set_file"),
		sll_api_log_set_file,
		SLL_CHAR("#sBb|b")
	},
	{
		SLL_CHAR("sll:log_set_function"),
		sll_api_log_set_function,
		SLL_CHAR("#s#sBb|b")
	},
	{
		SLL_CHAR("sll:map_extend"),
		sll_api_map_extend,
		SLL_CHAR("m#m|")
	},
	{
		SLL_CHAR("sll:map_remove"),
		sll_api_map_remove,
		SLL_CHAR("mo|D")
	},
	{
		SLL_CHAR("sll:math_abs"),
		sll_api_math_abs,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_acos"),
		sll_api_math_acos,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_acosh"),
		sll_api_math_acosh,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_asin"),
		sll_api_math_asin,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_asinh"),
		sll_api_math_asinh,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_atan"),
		sll_api_math_atan,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_atan2"),
		sll_api_math_atan2,
		SLL_CHAR("ff|f")
	},
	{
		SLL_CHAR("sll:math_atanh"),
		sll_api_math_atanh,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_cbrt"),
		sll_api_math_cbrt,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_ceil"),
		sll_api_math_ceil,
		SLL_CHAR("f|i")
	},
	{
		SLL_CHAR("sll:math_combinations"),
		sll_api_math_combinations,
		SLL_CHAR("ii|i")
	},
	{
		SLL_CHAR("sll:math_copy_sign"),
		sll_api_math_copy_sign,
		SLL_CHAR("ff|f")
	},
	{
		SLL_CHAR("sll:math_cos"),
		sll_api_math_cos,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_cosh"),
		sll_api_math_cosh,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_euler_phi"),
		sll_api_math_euler_phi,
		SLL_CHAR("i|i")
	},
	{
		SLL_CHAR("sll:math_exp"),
		sll_api_math_exp,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_factorial"),
		sll_api_math_factorial,
		SLL_CHAR("i|i")
	},
	{
		SLL_CHAR("sll:math_factors"),
		sll_api_math_factors,
		SLL_CHAR("i|a")
	},
	{
		SLL_CHAR("sll:math_floor"),
		sll_api_math_floor,
		SLL_CHAR("f|i")
	},
	{
		SLL_CHAR("sll:math_gcd"),
		sll_api_math_gcd,
		SLL_CHAR("ii|i")
	},
	{
		SLL_CHAR("sll:math_int_log2"),
		sll_api_math_int_log2,
		SLL_CHAR("i|i")
	},
	{
		SLL_CHAR("sll:math_int_pow"),
		sll_api_math_int_pow,
		SLL_CHAR("iii|i")
	},
	{
		SLL_CHAR("sll:math_int_sqrt"),
		sll_api_math_int_sqrt,
		SLL_CHAR("i|i")
	},
	{
		SLL_CHAR("sll:math_log"),
		sll_api_math_log,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_log10"),
		sll_api_math_log10,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_log2"),
		sll_api_math_log2,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_permutations"),
		sll_api_math_permutations,
		SLL_CHAR("ii|i")
	},
	{
		SLL_CHAR("sll:math_pow"),
		sll_api_math_pow,
		SLL_CHAR("ff|f")
	},
	{
		SLL_CHAR("sll:math_round"),
		sll_api_math_round,
		SLL_CHAR("f|i")
	},
	{
		SLL_CHAR("sll:math_sin"),
		sll_api_math_sin,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_sinh"),
		sll_api_math_sinh,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_sqrt"),
		sll_api_math_sqrt,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_tan"),
		sll_api_math_tan,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:math_tanh"),
		sll_api_math_tanh,
		SLL_CHAR("f|f")
	},
	{
		SLL_CHAR("sll:object_new"),
		sll_api_object_new,
		SLL_CHAR("#so!|o")
	},
	{
		SLL_CHAR("sll:path_absolute"),
		sll_api_path_absolute,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:path_exists"),
		sll_api_path_exists,
		SLL_CHAR("#s|b")
	},
	{
		SLL_CHAR("sll:path_get_cwd"),
		sll_api_path_get_cwd,
		SLL_CHAR("|~s")
	},
	{
		SLL_CHAR("sll:path_is_dir"),
		sll_api_path_is_dir,
		SLL_CHAR("#s|b")
	},
	{
		SLL_CHAR("sll:path_join"),
		sll_api_path_join,
		SLL_CHAR("#s!|s")
	},
	{
		SLL_CHAR("sll:path_list_dir"),
		sll_api_path_list_dir,
		SLL_CHAR("#sb|~a")
	},
	{
		SLL_CHAR("sll:path_mkdir"),
		sll_api_path_mkdir,
		SLL_CHAR("#sb|i")
	},
	{
		SLL_CHAR("sll:path_relative"),
		sll_api_path_relative,
		SLL_CHAR("#s#s|s")
	},
	{
		SLL_CHAR("sll:path_set_cwd"),
		sll_api_path_set_cwd,
		SLL_CHAR("#s|i")
	},
	{
		SLL_CHAR("sll:path_size"),
		sll_api_path_size,
		SLL_CHAR("#s|i")
	},
	{
		SLL_CHAR("sll:path_split"),
		sll_api_path_split,
		SLL_CHAR("#s|a")
	},
	{
		SLL_CHAR("sll:path_split_drive"),
		sll_api_path_split_drive,
		SLL_CHAR("#s|a")
	},
	{
		SLL_CHAR("sll:process_execute_shell"),
		sll_api_process_execute_shell,
		SLL_CHAR("#s|i")
	},
	{
		SLL_CHAR("sll:process_get_pid"),
		sll_api_process_get_pid,
		SLL_CHAR("|i")
	},
	{
		SLL_CHAR("sll:process_join"),
		sll_api_process_join,
		SLL_CHAR("#s+|s")
	},
	{
		SLL_CHAR("sll:process_split"),
		sll_api_process_split,
		SLL_CHAR("#s|a")
	},
	{
		SLL_CHAR("sll:process_start"),
		sll_api_process_start,
		SLL_CHAR("#a#sB#s|a")
	},
	{
		SLL_CHAR("sll:random_get_float"),
		sll_api_random_get_float,
		SLL_CHAR("ff|f")
	},
	{
		SLL_CHAR("sll:random_get_int"),
		sll_api_random_get_int,
		SLL_CHAR("ii|i")
	},
	{
		SLL_CHAR("sll:random_get_string"),
		sll_api_random_get_string,
		SLL_CHAR("Dcc|s")
	},
	{
		SLL_CHAR("sll:serial_decode_float"),
		sll_api_serial_decode_float,
		SLL_CHAR("i|~f")
	},
	{
		SLL_CHAR("sll:serial_decode_integer"),
		sll_api_serial_decode_integer,
		SLL_CHAR("i|~a")
	},
	{
		SLL_CHAR("sll:serial_decode_object"),
		sll_api_serial_decode_object,
		SLL_CHAR("i|~a")
	},
	{
		SLL_CHAR("sll:serial_decode_signed_integer"),
		sll_api_serial_decode_signed_integer,
		SLL_CHAR("i|~a")
	},
	{
		SLL_CHAR("sll:serial_decode_string"),
		sll_api_serial_decode_string,
		SLL_CHAR("i|~s")
	},
	{
		SLL_CHAR("sll:serial_encode_float"),
		sll_api_serial_encode_float,
		SLL_CHAR("if|Q")
	},
	{
		SLL_CHAR("sll:serial_encode_integer"),
		sll_api_serial_encode_integer,
		SLL_CHAR("iQ|Q")
	},
	{
		SLL_CHAR("sll:serial_encode_object"),
		sll_api_serial_encode_object,
		SLL_CHAR("io!|Q")
	},
	{
		SLL_CHAR("sll:serial_encode_signed_integer"),
		sll_api_serial_encode_signed_integer,
		SLL_CHAR("ii|Q")
	},
	{
		SLL_CHAR("sll:serial_encode_string"),
		sll_api_serial_encode_string,
		SLL_CHAR("i#s|Q")
	},
	{
		SLL_CHAR("sll:sort_quicksort"),
		sll_api_sort_quicksort,
		SLL_CHAR("&abbi|o")
	},
	{
		SLL_CHAR("sll:string_checksum"),
		sll_api_string_checksum,
		SLL_CHAR("#s|D")
	},
	{
		SLL_CHAR("sll:string_convert"),
		sll_api_string_convert,
		SLL_CHAR("o!|s")
	},
	{
		SLL_CHAR("sll:string_count"),
		sll_api_string_count,
		SLL_CHAR("#sy|i")
	},
	{
		SLL_CHAR("sll:string_count_left"),
		sll_api_string_count_left,
		SLL_CHAR("#sc|i")
	},
	{
		SLL_CHAR("sll:string_count_right"),
		sll_api_string_count_right,
		SLL_CHAR("#sc|i")
	},
	{
		SLL_CHAR("sll:string_ends"),
		sll_api_string_ends,
		SLL_CHAR("#sy|b")
	},
	{
		SLL_CHAR("sll:string_flip_case"),
		sll_api_string_flip_case,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:string_format"),
		sll_api_string_format,
		SLL_CHAR("#so!|s")
	},
	{
		SLL_CHAR("sll:string_index"),
		sll_api_string_index,
		SLL_CHAR("#syD|i")
	},
	{
		SLL_CHAR("sll:string_index_list"),
		sll_api_string_index_list,
		SLL_CHAR("#syb|i")
	},
	{
		SLL_CHAR("sll:string_index_reverse"),
		sll_api_string_index_reverse,
		SLL_CHAR("#sy|i")
	},
	{
		SLL_CHAR("sll:string_index_reverse_list"),
		sll_api_string_index_reverse_list,
		SLL_CHAR("#syb|i")
	},
	{
		SLL_CHAR("sll:string_join"),
		sll_api_string_join,
		SLL_CHAR("y#a|s")
	},
	{
		SLL_CHAR("sll:string_pad"),
		sll_api_string_pad,
		SLL_CHAR("#sDc|s")
	},
	{
		SLL_CHAR("sll:string_pad_left"),
		sll_api_string_pad_left,
		SLL_CHAR("#sDc|s")
	},
	{
		SLL_CHAR("sll:string_pad_right"),
		sll_api_string_pad_right,
		SLL_CHAR("#sDc|s")
	},
	{
		SLL_CHAR("sll:string_replace"),
		sll_api_string_replace,
		SLL_CHAR("#syy|s")
	},
	{
		SLL_CHAR("sll:string_reverse"),
		sll_api_string_reverse,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:string_secure_equal"),
		sll_api_string_secure_equal,
		SLL_CHAR("#s#s|b")
	},
	{
		SLL_CHAR("sll:string_split"),
		sll_api_string_split,
		SLL_CHAR("#sy|a")
	},
	{
		SLL_CHAR("sll:string_starts"),
		sll_api_string_starts,
		SLL_CHAR("#sy|b")
	},
	{
		SLL_CHAR("sll:string_to_lower_case"),
		sll_api_string_to_lower_case,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:string_to_title_case"),
		sll_api_string_to_title_case,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:string_to_upper_case"),
		sll_api_string_to_upper_case,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:string_trim"),
		sll_api_string_trim,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:string_trim_left"),
		sll_api_string_trim_left,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:string_trim_right"),
		sll_api_string_trim_right,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("sll:struct_double_from_bits"),
		sll_api_struct_double_from_bits,
		SLL_CHAR("Q|f")
	},
	{
		SLL_CHAR("sll:struct_double_to_bits"),
		sll_api_struct_double_to_bits,
		SLL_CHAR("f|Q")
	},
	{
		SLL_CHAR("sll:struct_float_from_bits"),
		sll_api_struct_float_from_bits,
		SLL_CHAR("D|f")
	},
	{
		SLL_CHAR("sll:struct_float_to_bits"),
		sll_api_struct_float_to_bits,
		SLL_CHAR("f|D")
	},
	{
		SLL_CHAR("sll:sys_get_args"),
		sll_api_sys_get_args,
		SLL_CHAR("|a")
	},
	{
		SLL_CHAR("sll:sys_get_cpu_count"),
		sll_api_sys_get_cpu_count,
		SLL_CHAR("|W")
	},
	{
		SLL_CHAR("sll:sys_get_env"),
		sll_api_sys_get_env,
		SLL_CHAR("|m")
	},
	{
		SLL_CHAR("sll:sys_get_executable"),
		sll_api_sys_get_executable,
		SLL_CHAR("|s")
	},
	{
		SLL_CHAR("sll:sys_get_library"),
		sll_api_sys_get_library,
		SLL_CHAR("|s")
	},
	{
		SLL_CHAR("sll:sys_get_platform"),
		sll_api_sys_get_platform,
		SLL_CHAR("|s")
	},
	{
		SLL_CHAR("sll:sys_get_sandbox_flags"),
		sll_api_sys_get_sandbox_flags,
		SLL_CHAR("|Q")
	},
	{
		SLL_CHAR("sll:sys_get_version"),
		sll_api_sys_get_version,
		SLL_CHAR("|a")
	},
	{
		SLL_CHAR("sll:sys_load_library"),
		sll_api_sys_load_library,
		SLL_CHAR("#sQQQQQ|b")
	},
	{
		SLL_CHAR("sll:sys_remove_env"),
		sll_api_sys_remove_env,
		SLL_CHAR("#s|")
	},
	{
		SLL_CHAR("sll:sys_set_env"),
		sll_api_sys_set_env,
		SLL_CHAR("#s#s|")
	},
	{
		SLL_CHAR("sll:sys_set_sandbox_flag"),
		sll_api_sys_set_sandbox_flag,
		SLL_CHAR("B|")
	},
	{
		SLL_CHAR("sll:thread_create"),
		sll_api_thread_create,
		SLL_CHAR("i#a|i")
	},
	{
		SLL_CHAR("sll:thread_create_barrier"),
		sll_api_thread_create_barrier,
		SLL_CHAR("|i")
	},
	{
		SLL_CHAR("sll:thread_create_lock"),
		sll_api_thread_create_lock,
		SLL_CHAR("|i")
	},
	{
		SLL_CHAR("sll:thread_create_semaphore"),
		sll_api_thread_create_semaphore,
		SLL_CHAR("D|i")
	},
	{
		SLL_CHAR("sll:thread_delete"),
		sll_api_thread_delete,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:thread_delete_barrier"),
		sll_api_thread_delete_barrier,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:thread_delete_lock"),
		sll_api_thread_delete_lock,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:thread_delete_semaphore"),
		sll_api_thread_delete_semaphore,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:thread_get_internal_data"),
		sll_api_thread_get_internal_data,
		SLL_CHAR("D|a")
	},
	{
		SLL_CHAR("sll:thread_increase_barrier"),
		sll_api_thread_increase_barrier,
		SLL_CHAR("D|i")
	},
	{
		SLL_CHAR("sll:thread_release_lock"),
		sll_api_thread_release_lock,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:thread_release_semaphore"),
		sll_api_thread_release_semaphore,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:thread_reset_barrier"),
		sll_api_thread_reset_barrier,
		SLL_CHAR("D|i")
	},
	{
		SLL_CHAR("sll:thread_restart"),
		sll_api_thread_restart,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:thread_suspend"),
		sll_api_thread_suspend,
		SLL_CHAR("D|b")
	},
	{
		SLL_CHAR("sll:time_current"),
		sll_api_time_current,
		SLL_CHAR("|f")
	},
	{
		SLL_CHAR("sll:time_current_ns"),
		sll_api_time_current_ns,
		SLL_CHAR("|Q")
	},
	{
		SLL_CHAR("sll:time_sleep"),
		sll_api_time_sleep,
		SLL_CHAR("x|f")
	},
	{
		SLL_CHAR("sll:time_sleep_ns"),
		sll_api_time_sleep_ns,
		SLL_CHAR("x|Q")
	},
	{
		SLL_CHAR("sll:vm_get_config"),
		sll_api_vm_get_config,
		SLL_CHAR("|a")
	},
	{
		SLL_CHAR("sll:vm_get_instruction_count"),
		sll_api_vm_get_instruction_count,
		SLL_CHAR("|Q")
	},
	{
		SLL_CHAR("sll:vm_get_instruction_index"),
		sll_api_vm_get_instruction_index,
		SLL_CHAR("|D")
	},
	{
		SLL_CHAR("sll:vm_get_location"),
		sll_api_vm_get_location,
		SLL_CHAR("D|a")
	},
	{
		SLL_CHAR("sll:vm_get_ref_count"),
		sll_api_vm_get_ref_count,
		SLL_CHAR("o|D")
	},
	{
		SLL_CHAR("sll:weakref__init"),
		sll_api_weakref__init,
		SLL_CHAR("oi|")
	},
	{
		SLL_CHAR("sll:weakref_create"),
		sll_api_weakref_create,
		SLL_CHAR("o|Q")
	},
	{
		SLL_CHAR("sll:weakref_delete"),
		sll_api_weakref_delete,
		SLL_CHAR("Q|b")
	},
	{
		SLL_CHAR("sll:weakref_get"),
		sll_api_weakref_get,
		SLL_CHAR("Q|o")
	},
	{
		SLL_CHAR("sll:weakref_set_callback_data"),
		sll_api_weakref_set_callback_data,
		SLL_CHAR("Qo|")
	}
};



__SLL_EXTERNAL const sll_internal_function_descriptor_t* sll_builtin_internal_function_data=(const sll_internal_function_descriptor_t*)(&_ifunc_data_ptr);
__SLL_EXTERNAL const sll_function_index_t sll_builtin_internal_function_count=195;

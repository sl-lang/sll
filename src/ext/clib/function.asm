[BITS 64]
[SECTION .text]
[DEFAULT rel]



%ifdef __SLL_BUILD_DARWIN
%define __SYMBOL(nm) _ %+ nm
%define __CALL(nm) _ %+ nm
%define __EXTERNAL(nm) extern _ %+ nm
%elifdef __SLL_BUILD_WINDOWS
%define __SYMBOL(nm) nm
%define __CALL(nm) QWORD [__imp_ %+ nm]
%define __EXTERNAL(nm) extern __imp_ %+ nm
%else
%define __SYMBOL(nm) nm
%define __CALL(nm) nm wrt ..plt
%define __EXTERNAL(nm) extern nm
%endif



%define sll_array_t.length(x) DWORD [x + 0]
%define sll_array_t.data(x) QWORD [x + 8]
%define sll_object_t.data(x) QWORD [x + 24]



__EXTERNAL(sll_int_to_object)
__EXTERNAL(sll_float_to_object)
%ifdef __SLL_BUILD_WINDOWS
extern __chkstk
%endif



global __SYMBOL(clib_api_function_call)
__SYMBOL(clib_api_function_call):
	; rcx - Function pointer
	; rdx - Return typ
	; r8 - Argument array
	push rbp
	mov rbp, rsp
	push rbx

	; rcx - Function pointer
	; rdx - Return type
	; r8 - Argument array
%ifdef DEBUG_BUILD
	mov QWORD [rbp+16], rcx
	mov QWORD [rbp+24], rdx
	mov QWORD [rbp+32], r8
%endif

	; rbx - Return type
	; rcx - Function pointer
	; rdx - Return type
	; r8 - Argument array
	mov rbx, rdx

	; rax - Stack offset
	; rbx - Return type
	; rcx - Function pointer
	; r8 - Argument array
	mov eax, sll_array_t.length(r8)
	add eax, 1
	and eax, 0xfffffffe
	mov edx, 8
	cmp eax, 8
	cmovb eax, edx
	shl rax, 3
%ifdef __SLL_BUILD_WINDOWS
	cmp rax, 4096
	jb ._small_stack
	call __chkstk
._small_stack:
%endif
	sub rsp, rax

	; rax - Argument count
	; rbx - Return type
	; rcx - Function pointer
	; rdx - Stack pointer for arguments
	; r8 - Argument array / Argument object pointer
	; r9 - Object pointer
	mov eax, sll_array_t.length(r8)
	test eax, eax
	jz ._no_args
	mov rdx, rsp
	mov r8, sll_array_t.data(r8)
._next_arg:
	mov r9, QWORD [r8]
	add r8, 8
	mov r9, sll_object_t.data(r9)
	mov QWORD [rdx], r9
	add rdx, 8
	sub eax, 1
	jnz ._next_arg
._no_args:

	; rax - Function pointer
	; rbx - Return type
	; rcx - Function pointer
	mov rax, rcx
	btr rax, 63
	jc ._system_v_calling_convention
	mov rcx, QWORD [rsp]
	mov rdx, QWORD [rsp+8]
	mov r8, QWORD [rsp+16]
	mov r9, QWORD [rsp+24]
	movq xmm0, rcx
	movq xmm1, rdx
	movq xmm2, r8
	movq xmm3, r9
	jmp ._call_function
._system_v_calling_convention:
	mov rdi, QWORD [rsp]
	mov rsi, QWORD [rsp+8]
	mov rdx, QWORD [rsp+16]
	mov rcx, QWORD [rsp+24]
	mov r8, QWORD [rsp+32]
	mov r9, QWORD [rsp+40]
	movq xmm0, rdi
	movq xmm1, rsi
	movq xmm2, rdx
	movq xmm3, rcx
	movq xmm4, r8
	movq xmm5, r9
	movq xmm6, QWORD [rsp + 48]
	movq xmm7, QWORD [rsp + 56]
._call_function:
	call rax

	; rax - Integer return value
	; rbx - Return type
	; xmm0 - Floating-point return value
	test rbx, rbx
	mov rbx, QWORD [rbp-8]
	leave

	; rax - Integer return value
	; xmm0 - Floating-point return value
	jnz ._return_float
%ifdef __SLL_BUILD_WINDOWS
	mov rcx, rax
%else
	mov rdi, rax
%endif
	jmp __CALL(sll_int_to_object)
._return_float:
	jmp __CALL(sll_float_to_object)

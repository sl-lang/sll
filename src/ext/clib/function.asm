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
	; rdx - Return type
	; r8 - Argument array
	push rbp
	mov rbp, rsp
	push rbx
%ifndef __SLL_BUILD_WINDOWS
	push rdi
	push rsi
	sub rsp, 168
	vmovaps OWORD [rsp], xmm6
	vmovaps OWORD [rsp+16], xmm7
	vmovaps OWORD [rsp+32], xmm8
	vmovaps OWORD [rsp+48], xmm9
	vmovaps OWORD [rsp+64], xmm10
	vmovaps OWORD [rsp+80], xmm11
	vmovaps OWORD [rsp+96], xmm12
	vmovaps OWORD [rsp+112], xmm13
	vmovaps OWORD [rsp+128], xmm14
	vmovaps OWORD [rsp+144], xmm15
%endif

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
	bt ebx, 1
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
	bt ebx, 0
	jnc ._return_float
%ifdef __SLL_BUILD_WINDOWS
	mov rcx, rax
%else
	mov rdi, rax
%endif
	call __CALL(sll_int_to_object)
._cleanup:
%ifndef __SLL_BUILD_WINDOWS
	lea rsp, [rbp-192]
	vmovaps xmm6, OWORD [rsp]
	vmovaps xmm7, OWORD [rsp+16]
	vmovaps xmm8, OWORD [rsp+32]
	vmovaps xmm9, OWORD [rsp+48]
	vmovaps xmm10, OWORD [rsp+64]
	vmovaps xmm11, OWORD [rsp+80]
	vmovaps xmm12, OWORD [rsp+96]
	vmovaps xmm13, OWORD [rsp+112]
	vmovaps xmm14, OWORD [rsp+128]
	vmovaps xmm15, OWORD [rsp+144]
	add rsp, 168
	pop rsi
	pop rdi
%endif
	mov rbx, QWORD [rbp-8]
	leave
	ret
._return_float:
	call __CALL(sll_float_to_object)
	jmp ._cleanup

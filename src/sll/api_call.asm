[BITS 64]
[SECTION .text]
[DEFAULT rel]
%include "sll/_internal/common.inc"



__SLL_EXPORT _call_api_func_assembly
	; rcx - Return value pointer
	; rdx - Argument bitmap pointer
	; r8 - Argument data pointer
	; r9 - Argument count
	; [rsp+48] - Function pointer
	push rbp
	mov rbp, rsp
	push rsi

	mov QWORD [rbp+16], rcx
%ifdef DEBUG_BUILD
	mov QWORD [rbp+24], rdx
	mov QWORD [rbp+32], r8
	mov QWORD [rbp+40], r9
%endif

	; rax - Stack offset
	; rdx - Argument bitmap pointer
	; r8 - Argument data pointer
	; r9 - Argument count
	; [rbp+16] - Return value structure pointer
	; [rbp+48] - Function pointer
	mov eax, r9d
	add eax, 1
	and eax, 0xfffffffe
	mov ecx, 4
	cmp eax, 4
	cmovb eax, ecx
	lea rax, [rax*8+8]
%ifdef __SLL_BUILD_WINDOWS
	cmp rax, 4096
	jb ._small_stack
	call __chkstk
._small_stack:
%endif
	sub rsp, rax

	; rax - Number of arguments left in current bitmap
	; rdx - Argument bitmap pointer
	; rsi - Current bitmap
	; r8 - Argument data pointer
	; r9 - Argument count
	; r10 - Stack pointer for arguments
	; [rbp+16] - Return value structure pointer
	; [rbp+48] - Function pointer
	mov al, 32
	mov rsi, QWORD [rdx]
	mov r10, rsp
	test r9d, r9d
	jz ._no_args
._next_arg:
	mov ecx, esi
	and ecx, 3
	lea rcx, [r8+rcx*8+8]
	jnz ._skip_dereference
	mov r8, QWORD [r8]
._skip_dereference:
	mov QWORD [r10], r8
	shr rsi, 2
	mov r8, rcx
	add r10, 8
	sub al, 1
	jnz ._check_end
	add rdx, 8
	mov rsi, QWORD [rdx]
	mov al, 32
._check_end:
	sub r9d, 1
	jnz ._next_arg
._no_args:
	mov rcx, QWORD [rbp+16]
	mov QWORD [r10], rcx

	; [rbp+16] - Return value structure pointer
	; [rbp+48] - Function pointer
	mov rcx, QWORD [rsp]
	mov rdx, QWORD [rsp+8]
	mov r8, QWORD [rsp+16]
	mov r9, QWORD [rsp+24]
	mov r10, QWORD [rbp+48]
	movq xmm0, rcx
	movq xmm1, rdx
	movq xmm2, r8
	movq xmm3, r9
	call r10

	; rax - Integer return value
	; rsi - Current bitmap
	; xmm0 - Floating-point return value
	; [rbp+16] - Return value structure pointer
	test sil, sil
	jz ._register_return_value
	sub sil, 1
	jz ._return
	xorpd xmm0, xmm0
	add rax, 1
	jz ._return
	sub rax, 1
	mov rcx, 0x3ff0000000000000
	movq xmm0, rcx
._register_return_value:
	mov rcx, QWORD [rbp+16]
	mov QWORD [rcx], rax
._return:

	; xmm0 - Floating-point return value
	mov rsi, QWORD [rbp-8]
	leave
	ret

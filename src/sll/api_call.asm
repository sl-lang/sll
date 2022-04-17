[BITS 64]
section .text



%ifdef __SLL_BUILD_DARWIN
%define __C_FUNC(nm) _ %+ nm
%else
%define __C_FUNC(nm) nm
%endif



; eax - Counter of arguments left in current bitmap
; rbx - Return value pointer
; rcx - Current bitmap
; rdx - Argument bitmap pointer
; rsi - Temporary register
; r8 - Argument data pointer
; r9d - Argument count
; r10 - Function pointer
; r11 - Stack pointer for arguments
global __C_FUNC(_call_api_func)
__C_FUNC(_call_api_func):
	push rbx
	push rsi
	push rbp
	mov rbp, rsp

	mov rbx, rcx
	mov r10, QWORD [rsp+64]

	mov rax, r9
	add rax, 2
	and rax, ~1
	cmp rax, 4
	jae ._skip_stack_padding
	mov rax, 4
._skip_stack_padding:
	shl rax, 3
	sub rsp, rax

	mov eax, 64
	mov rcx, QWORD [rdx]
	mov r11, rsp
	test r9d, r9d
	jz ._no_args
._next_arg:

	bt cx, 0
	jc ._push_wide_arg
	mov rsi, QWORD [r8]
	mov QWORD [r11], rsi
	jmp ._consume_arg
._push_wide_arg:
	mov QWORD [r11], r8
	add r8, 8

._consume_arg:
	shr rcx, 1
	add r8, 8
	add r11, 8
	sub eax, 1
	jnz ._check_end
	add rdx, 8
	mov rcx, QWORD [rdx]
	mov eax, 64
._check_end:
	sub r9d, 1
	jnz ._next_arg
._no_args:

	mov sil, cl
	and sil, 1
	jz ._no_return_arg
	mov QWORD [r11], rbx
._no_return_arg:

	mov rcx, QWORD [rsp]
	mov rdx, QWORD [rsp+8]
	mov r8, QWORD [rsp+16]
	mov r9, QWORD [rsp+24]
	movq xmm0, rcx
	movq xmm1, rdx
	movq xmm2, r8
	movq xmm3, r9
	call r10

	test sil, sil
	jnz ._no_return_value
	mov QWORD [rbx], rax
._no_return_value:

	leave
	pop rsi
	pop rbx
	ret

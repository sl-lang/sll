[BITS 64]
section .text



%ifdef __SLL_BUILD_DARWIN
%define __C_FUNC(nm) _ %+ nm
%else
%define __C_FUNC(nm) nm
%endif



%ifdef __SLL_BUILD_WINDOWS
extern __chkstk
%endif



; eax - Number of arguments left in current bitmap
; rbx - Return value pointer
; rcx - Temporary register
; rdx - Argument bitmap pointer
; rsi - Current bitmap
; r8 - Argument data pointer
; r9d - Argument count
; r10 - Stack pointer for arguments
; r11 - Function pointer
global __C_FUNC(_call_api_func)
__C_FUNC(_call_api_func):
	push rbx
	push rsi
	push rbp
	mov rbp, rsp

	mov rbx, rcx
	mov r11, QWORD [rsp+64]

	mov eax, r9d
	add eax, 2
	and eax, 0xfffffffe
	cmp eax, 4
	jae ._skip_stack_padding
	mov eax, 4
._skip_stack_padding:
	shl rax, 3
%ifdef __SLL_BUILD_WINDOWS
	cmp rax, 4096
	jb ._skip_stack_check
	call __chkstk
._skip_stack_check:
%endif
	sub rsp, rax

	mov al, 64
	mov rsi, QWORD [rdx]
	mov r10, rsp
	test r9d, r9d
	jz ._no_args
._next_arg:

	bt si, 0
	jc ._push_wide_arg
	mov rcx, QWORD [r8]
	mov QWORD [r10], rcx
	jmp ._consume_arg
._push_wide_arg:
	mov QWORD [r10], r8
	add r8, 8

._consume_arg:
	shr rsi, 1
	add r8, 8
	add r10, 8
	sub al, 1
	jnz ._check_end
	add rdx, 8
	mov rsi, QWORD [rdx]
	mov al, 64
._check_end:
	sub r9d, 1
	jnz ._next_arg
._no_args:

	and sil, 1
	jz ._no_return_arg
	mov QWORD [r10], rbx
._no_return_arg:

	mov rcx, QWORD [rsp]
	mov rdx, QWORD [rsp+8]
	mov r8, QWORD [rsp+16]
	mov r9, QWORD [rsp+24]
	movq xmm0, rcx
	movq xmm1, rdx
	movq xmm2, r8
	movq xmm3, r9
	call r11

	test sil, sil
	jnz ._no_return_value
	mov QWORD [rbx], rax
._no_return_value:

	leave
	pop rsi
	pop rbx
	ret

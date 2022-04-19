[BITS 64]
section .text
%include "sll/_internal/common.inc"



; eax - Number of arguments left in current bitmap
; rbx - Return value pointer
; rcx - Temporary register
; rdx - Argument bitmap pointer
; rsi - Current bitmap
; r8 - Argument data pointer
; r9d - Argument count
; r10 - Stack pointer for arguments
; r11 - Function pointer
__SLL_EXPORT _call_api_func_assembly
	push rbx
	push rsi
	push rbp
	mov rbp, rsp

	mov rbx, rcx
	mov r11, QWORD [rsp+64]

	mov eax, r9d
	add eax, 2
	and eax, 0xfffffffe
	mov ecx, 4
	cmp eax, ecx
	cmovb eax, ecx
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
	jmp ._load_next_arg
._push_wide_arg:
	mov QWORD [r10], r8
	add r8, 8

._load_next_arg:
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

	cmp al, 1
	jg ._skip_extra_load
	add rdx, 8
	mov rcx, QWORD [rdx]
	shr cl, 1
	or sil, cl
._skip_extra_load:

	mov QWORD [r10], rbx

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
	jz ._register_return_value
	sub sil, 1
	jz ._cleanup
	xor rcx,rcx
	xorpd xmm0, xmm0
	add rax, 1
	setnz cl
	cvtsi2sd xmm0, rax
	jz ._cleanup
._register_return_value:
	mov QWORD [rbx], rax
._cleanup:

	leave
	pop rsi
	pop rbx
	ret

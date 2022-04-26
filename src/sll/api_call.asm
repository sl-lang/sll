[BITS 64]
section .text
%include "sll/_internal/common.inc"



__SLL_EXPORT _call_api_func_assembly
	; rcx - Return value pointer
	; rdx - Argument bitmap pointer
	; r8 - Argument data pointer
	; r9 - Argument count
	; [rsp+40] - Function pointer
	push rbx
	push rsi
	push rbp
	mov rbp, rsp

%ifdef DEBUG_BUILD
	mov QWORD [rsp+32], rcx
	mov QWORD [rsp+40], rdx
	mov QWORD [rsp+48], r8
	mov QWORD [rsp+56], r9
%endif

	; rbx - Return value pointer
	; rdx - Argument bitmap pointer
	; r8 - Argument data pointer
	; r9 - Argument count
	; r11 - Function pointer
	; [rsp+64] - Function pointer
	mov rbx, rcx
	mov r11, QWORD [rsp+64]

	; rax - Stack offset
	; rbx - Return value pointer
	; rcx - Temporary register
	; rdx - Argument bitmap pointer
	; r8 - Argument data pointer
	; r9 - Argument count
	; r11 - Function pointer
	mov eax, r9d
	add eax, 2
	and eax, 0xfffffffe
	mov ecx, 4
	cmp eax, ecx
	jae ._above_min_size
	mov eax, ecx
._above_min_size:
	shl rax, 3
%ifdef __SLL_BUILD_WINDOWS
	cmp rax, 4096
	jb ._skip_stack_check
	mov rcx, r11
	call __chkstk
	mov r11, rcx
._skip_stack_check:
%endif
	sub rsp, rax

	; rax - Number of arguments left in current bitmap
	; rbx - Return value pointer
	; rcx - Temporary register
	; rdx - Argument bitmap pointer
	; rsi - Current bitmap
	; r8 - Argument data pointer
	; r9 - Argument count
	; r10 - Stack pointer for arguments
	; r11 - Function pointer
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

	; rax - Number of arguments left in current bitmap
	; rbx - Return value pointer
	; rdx - Argument bitmap pointer
	; rsi - Current bitmap
	; r10 - Stack pointer for arguments
	; r11 - Function pointer
	mov QWORD [r10], rbx

	; rax - Number of arguments left in current bitmap
	; rbx - Return value pointer
	; rcx - Temporary register
	; rdx - Argument bitmap pointer
	; rsi - Current bitmap
	; r11 - Function pointer
	cmp al, 1
	jg ._skip_extra_load
	add rdx, 8
	mov rcx, QWORD [rdx]
	shr cl, 1
	or sil, cl
._skip_extra_load:

	mov rcx, QWORD [rsp]
	mov rdx, QWORD [rsp+8]
	mov r8, QWORD [rsp+16]
	mov r9, QWORD [rsp+24]
	movq xmm0, rcx
	movq xmm1, rdx
	movq xmm2, r8
	movq xmm3, r9
	call r11

	; rax - Integer return value
	; rbx - Return value pointer
	; rcx - Temporary register
	; rsi - Current bitmap
	; xmm0 - Floating-point return value
	test sil, sil
	jz ._register_return_value
	sub sil, 1
	jz ._cleanup
	xor rcx, rcx
	xorpd xmm0, xmm0
	add rax, 1
	setnz cl
	cvtsi2sd xmm0, rcx
	jz ._cleanup
	sub rax, 1
._register_return_value:
	mov QWORD [rbx], rax
._cleanup:

	leave
	pop rsi
	pop rbx
	ret

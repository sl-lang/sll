[BITS 64]
section .text



%ifdef __SLL_BUILD_DARWIN
%define __C_FUNC(nm) _ %+ nm
%else
%define __C_FUNC(nm) nm
%endif



%define ARG_BITMAP_NORMAL 0
%define ARG_BITMAP_REF 1
%define ARG_BITMAP_WIDE 2

%define ARG_BITMAP_RETURN_NORMAL 0
%define ARG_BITMAP_RETURN_XMM 1
%define ARG_BITMAP_RETURN_REF 2



; eax - Counter of arguments left in current bitmap
; rbx - Return value pointer
; rcx - Current bitmap
; rdx - Argument bitmap pointer
; r8 - Argument data pointer
; r9d - Argument count
; r10 - Function pointer
; r11 - Stack pointer for arguments
global __C_FUNC(_call_api_func)
__C_FUNC(_call_api_func):
	push rbx
	push rbp
	mov rbp, rsp

	mov rbx, rcx
	mov r10, QWORD [rsp+56]

	mov rsi, QWORD [rsp+64]
	mov edi, DWORD [rsp+72]

	mov rax, r9
	add rax, 2
	and rax, ~1
	lea rax, [rax*8+8]
	sub rsp, rax

	mov rcx, QWORD [rsp+rax+64]
	mov QWORD [rsp], rcx
	mov ecx, DWORD [rsp+rax+72]
	mov DWORD [rsp+8], ecx

	mov eax, 32
	mov rcx, QWORD [rdx]
	mov r11, rsp
	test r9d, r9d
	jz ._no_args
._next_arg:

	;;;

	shr rcx, 2
	add r11, 8
	sub eax, 1
	jnz ._check_end
	add rdx, 8
	mov rcx, QWORD [rdx]
	mov eax, 32
._check_end:
	sub r9d, 1
	jnz ._next_arg
._no_args:

	mov BYTE [rbx], cl
	and ecx, ARG_BITMAP_RETURN_REF
	jz ._no_return_ref
	mov QWORD [r11], rbx
._no_return_ref:

	mov rcx, QWORD [rsp]
	mov rdx, QWORD [rsp+8]
	mov r8, QWORD [rsp+16]
	mov r9, QWORD [rsp+24]
	movq xmm0, rcx
	movq xmm1, rdx
	movq xmm2, r8
	movq xmm3, r9
	call r10

	mov cl, BYTE [rbx]
	mov QWORD [rbx], rax

	leave
	pop rbx
	ret

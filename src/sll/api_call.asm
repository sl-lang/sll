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



; rbx - Return value pointer
; rdx - Argument bitmap pointer
; r8 - Argument data pointer
; r9 - Argument count
; r10 - Function pointer
global __C_FUNC(_call_api_func)
__C_FUNC(_call_api_func):
	push rbx
	push rbp
	mov rbp, rsp

	mov rbx, rcx
	mov r10, QWORD [rsp+56]

	mov rcx, QWORD [rsp+64]
	mov edx, DWORD [rsp+72]

	mov rax, r9
	add rax, 2
	and rax, ~1
	lea rax, [rax*8+8]
	sub rsp, rax

	mov QWORD[rsp], rcx
	mov QWORD[rsp+8], rdx

	mov rcx, QWORD [rsp]
	mov rdx, QWORD [rsp+8]
	mov r8, QWORD [rsp+16]
	mov r9, QWORD [rsp+24]
	movq xmm0, rcx
	movq xmm1, rdx
	movq xmm2, r8
	movq xmm3, r9
	call r10

	mov QWORD [rbx], rax

	leave
	pop rbx
	ret

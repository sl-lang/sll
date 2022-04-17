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

%define MAX_ARGUMENTS_POW 24



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
	and rax, ((1<<MAX_ARGUMENTS_POW)-2)
	lea rax, [rax*8+8]
	sub rsp, rax

	mov QWORD[rsp], rcx
	mov QWORD[rsp+8], rdx

	mov rcx, QWORD [rsp]
	movq xmm0, rcx
	mov rdx, QWORD [rsp+8]
	movq xmm1, rdx
	mov r8, QWORD [rsp+16]
	movq xmm2, r8
	mov r9, QWORD [rsp+24]
	movq xmm3, r9
	call r10

	mov QWORD [rbx], rax

	leave
	pop rbx
	ret

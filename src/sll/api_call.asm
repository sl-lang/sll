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



global __C_FUNC(_call_api_func)
__C_FUNC(_call_api_func):
	push rbp
	mov rbp, rsp
	sub rsp, 32
	mov QWORD [rbp+16], rcx
	mov QWORD [rbp+24], rdx
	mov QWORD [rbp+32], r8
	mov QWORD [rbp+40], r9
	mov rax, QWORD [rbp+16]
	mov rcx, QWORD [rbp+40]
	mov edx, DWORD [rbp+48]
	call rax
	leave
	ret

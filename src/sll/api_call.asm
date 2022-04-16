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
	push rbx
	sub rsp, 32
	mov rax, rcx
	mov rcx, QWORD [rsp+80]
	mov edx, DWORD [rsp+88]
	mov rbx, r9
	call rax
	mov QWORD [rbx], rax
	add rsp, 32
	pop rbx
	ret

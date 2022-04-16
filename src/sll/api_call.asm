[BITS 64]
section .text



%ifdef __SLL_BUILD_DARWIN
%define FUNC(nm) _ %+ nm
%else
%define FUNC(nm) nm
%endif



global FUNC(_call_api_func)
FUNC(_call_api_func):
	push rbp
	mov rbp, rsp
	sub rsp, 32
	mov QWORD [rbp+16], rcx
	mov QWORD [rbp+24], rdx
	mov DWORD [rbp+32], r8d
	mov rax, QWORD [rbp+16]
	mov rcx, QWORD [rbp+24]
	mov edx, DWORD [rbp+32]
	call rax
	leave
	ret

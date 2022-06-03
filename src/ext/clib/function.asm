[BITS 64]
section .text
default rel



%ifdef __SLL_BUILD_DARWIN
%define __SYMBOL(nm) _ %+ nm
%else
%define __SYMBOL(nm) nm
%endif
%define __EXTERNAL_SYMBOL(nm) QWORD [__SYMBOL(nm) wrt ..got]



extern __SYMBOL(sll_static_int)



global __SYMBOL(clib_api_call_function)
__SYMBOL(clib_api_call_function):
	mov rax, __EXTERNAL_SYMBOL(sll_static_int)
	mov rax, QWORD [rax]
	add QWORD [rax], 1
	ret

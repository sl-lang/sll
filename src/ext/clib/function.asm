[BITS 64]
[SECTION .text]
[DEFAULT rel]



%ifdef __SLL_BUILD_DARWIN
%define __SYMBOL(nm) _ %+ nm
%else
%define __SYMBOL(nm) nm
%endif



global __SYMBOL(_call_function)
__SYMBOL(_call_function):
	xor rax, rax
	ret

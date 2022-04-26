[BITS 64]
section .text
%include "sll/_internal/common.inc"



%define CPUID_PROCESSOR_INFO 1
%define CPUID_EXTENDED_FEATURES 7

%define CPUID_AVX_BIT 27
%define CPUID_AVX_REGISTER ecx
%define CPUID_AVX2_BIT 5
%define CPUID_AVX2_REGISTER ebx



__SLL_EXPORT _check_cpuid_flags
%ifndef __SLL_BUILD_DARWIN
	mov r8, rbx
	mov eax, CPUID_PROCESSOR_INFO
	xor ecx, ecx
	cpuid
	bt CPUID_AVX_REGISTER, CPUID_AVX_BIT
	jnc ._return_error
	mov eax, CPUID_EXTENDED_FEATURES
	xor ecx, ecx
	cpuid
	bt CPUID_AVX2_REGISTER, CPUID_AVX2_BIT
	jnc ._return_error
	mov rbx, r8
%endif
	xor rax, rax
	ret
._return_error:
	mov eax, 1
	mov rbx, r8
	ret

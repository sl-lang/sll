[BITS 64]
section .text
%include "sll/_internal/common.inc"



%define CPUID_PROCESSOR_INFO 1
%define CPUID_EXTENDED_FEATURES 7

%define CPUID_AVX_TYPE CPUID_PROCESSOR_INFO
%define CPUID_AVX_BIT 27
%define CPUID_AVX_REGISTER ecx
%define CPUID_AVX2_TYPE CPUID_EXTENDED_FEATURES
%define CPUID_AVX2_BIT 5
%define CPUID_AVX2_REGISTER ebx



__SLL_EXPORT _check_cpuid_flags
	xor r9d, r9d
%ifndef __SLL_BUILD_DARWIN
	mov r8, rbx
	mov eax, CPUID_AVX_TYPE
	xor ecx, ecx
	cpuid
	shr CPUID_AVX_REGISTER, CPUID_AVX_BIT
	xor CPUID_AVX_REGISTER, 1
	or r9d, CPUID_AVX_REGISTER
	mov eax, CPUID_AVX2_TYPE
	xor ecx, ecx
	cpuid
	shr CPUID_AVX2_REGISTER, CPUID_AVX2_BIT
	xor CPUID_AVX2_REGISTER, 1
	or r9d, CPUID_AVX2_REGISTER
	mov rbx, r8
	and r9d, 1
%endif
	mov eax, r9d
	ret

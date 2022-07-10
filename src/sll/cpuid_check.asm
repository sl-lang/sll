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
	xor r8d, r8d
%ifndef __SLL_BUILD_DARWIN
	mov r9, rbx
	mov eax, CPUID_AVX_TYPE
	xor ecx, ecx
	cpuid
	bt CPUID_AVX_REGISTER, CPUID_AVX_BIT
	setnc r8b
	mov eax, CPUID_AVX2_TYPE
	xor ecx, ecx
	cpuid
	bt CPUID_AVX2_REGISTER, CPUID_AVX2_BIT
	setnc r8b
	mov rbx, r9
%endif
	mov eax, r8d
	ret

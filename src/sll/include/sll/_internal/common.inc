%ifdef __SLL_BUILD_DARWIN
%define __SLL_EXPORT_NAME(nm) _ %+ nm
%else
%define __SLL_EXPORT_NAME(nm) nm
%endif
%ifdef __SLL_BUILD_WINDOWS
extern __chkstk
%endif



%macro __SLL_EXPORT 1
	global __SLL_EXPORT_NAME(%1)
	__SLL_EXPORT_NAME(%1) %+ :
%endmacro

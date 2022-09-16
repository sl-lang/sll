#include <font/common.h>
#include <font/true_type.h>
#include <fontconfig/fontconfig.h>
#include <sll.h>



__FONT_API_CALL void font_api_true_type_find(const sll_string_t* name,sll_string_t* out){
	SLL_INIT_STRING(out);
	FcPattern* pattern=FcNameParse(name->data);
	FcConfigSubstitute(0,pattern,FcMatchPattern);
	FcDefaultSubstitute(pattern);
	FcResult tmp;
	FcPattern* match=FcFontMatch(NULL,pattern,&tmp);
	FcPatternDestroy(pattern);
	if (!match){
		return;
	}
	FcChar8* ptr;
	if (FcPatternGetString(match,FC_FILE,0,&ptr)==FcResultMatch){
		sll_string_from_pointer(SLL_CHAR(ptr),out);
	}
	FcPatternDestroy(match);
	FcFini();
}

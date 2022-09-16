#include <font/common.h>
#include <font/true_type.h>
#include <sll.h>
#include <stb_truetype.h>
#include <stdint.h>



__FONT_API_CALL void font_api_true_type_create(const sll_string_t* data,sll_string_t* out){
	sll_string_create(1048576,out);
	sll_zero_memory(out->data,1048576);
	stbtt_pack_context pack_ctx;
	stbtt_PackBegin(&pack_ctx,out->data,1024,1024,0,1,NULL);
	stbtt_PackSetOversampling(&pack_ctx,1,1);
	stbtt_packedchar render_data[95];
	stbtt_PackFontRange(&pack_ctx,data->data,0,32.0f,32,95,render_data);
	stbtt_PackEnd(&pack_ctx);
}

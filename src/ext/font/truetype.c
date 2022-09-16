#include <font/common.h>
#include <font/true_type.h>
#include <sll.h>
#include <stb_truetype.h>
#include <stdint.h>



__FONT_API_CALL void font_api_true_type_create(const sll_string_t* data,sll_array_t* out){
	uint8_t texture_data[1048576];
	stbtt_pack_context pack_ctx;
	stbtt_PackBegin(&pack_ctx,texture_data,1024,1024,0,1,NULL);
	stbtt_PackSetOversampling(&pack_ctx,1,1);
	stbtt_packedchar render_data[95];
	stbtt_PackFontRange(&pack_ctx,data->data,0,32.0f,32,95,render_data);
	stbtt_PackEnd(&pack_ctx);
	sll_new_object_array(SLL_CHAR("uul{MMMMFFFFF}"),out,1024,1024,texture_data,1048576,render_data,95ull,sizeof(stbtt_packedchar),SLL_OFFSETOF(stbtt_packedchar,x0),0xffffull,SLL_OFFSETOF(stbtt_packedchar,y0),0xffffull,SLL_OFFSETOF(stbtt_packedchar,x1),0xffffull,SLL_OFFSETOF(stbtt_packedchar,y1),0xffffull,SLL_OFFSETOF(stbtt_packedchar,xoff),SLL_OFFSETOF(stbtt_packedchar,yoff),SLL_OFFSETOF(stbtt_packedchar,xoff2),SLL_OFFSETOF(stbtt_packedchar,yoff2),SLL_OFFSETOF(stbtt_packedchar,xadvance));
}

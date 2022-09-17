#include <font/common.h>
#include <font/true_type.h>
#include <sll.h>
#include <stb_truetype.h>
#include <stdint.h>



__FONT_API_CALL void font_api_true_type_create(const sll_string_t* data,sll_array_t* out){
	sll_array_t glyphs=SLL_INIT_ARRAY_STRUCT;
	stbtt_fontinfo font;
	stbtt_InitFont(&font,data->data,0);
	int ascent;
	int descent;
	int line_gap;
	stbtt_GetFontVMetrics(&font,&ascent,&descent,&line_gap);
	float scale=stbtt_ScaleForPixelHeight(&font,64.0f);
	for (int i=32;i<127;i++){
		int glyph=stbtt_FindGlyphIndex(&font,i);
		if (!glyph){
			sll_array_push(NULL,sll_new_object(SLL_CHAR("00000Z")),&glyphs);
			continue;
		}
		int x0;
		int y0;
		int x1;
		int y1;
		stbtt_GetGlyphBitmapBox(&font,glyph,scale,scale,&x0,&y0,&x1,&y1);
		int advance;
		stbtt_GetGlyphHMetrics(&font,glyph,&advance,NULL);
		uint32_t width=x1-x0;
		uint32_t height=y1-y0;
		uint8_t* bitmap=sll_allocate_stack(width*height);
		stbtt_MakeGlyphBitmap(&font,bitmap,width,height,width,scale,scale,glyph);
		sll_array_push(NULL,sll_new_object(SLL_CHAR("hhuuul"),x0,y0,width,height,(int)(advance*scale),bitmap,width*height),&glyphs);
		sll_deallocate(bitmap);
	}
	sll_new_object_array(SLL_CHAR("hhha"),out,(int)(ascent*scale),(int)(descent*scale),(int)(line_gap*scale),&glyphs);
}

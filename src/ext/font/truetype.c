#include <font/common.h>
#include <font/true_type.h>
#include <sll.h>
#include <stb_truetype.h>
#include <stdint.h>



#define FONT_SIZE 64.0f



__FONT_API_CALL void font_api_true_type_create(const sll_string_t* data,sll_array_t* out){
	sll_array_t glyphs=SLL_INIT_ARRAY_STRUCT;
	stbtt_fontinfo font;
	stbtt_InitFont(&font,data->data,0);
	int ascent;
	int descent;
	int line_gap;
	stbtt_GetFontVMetrics(&font,&ascent,&descent,&line_gap);
	float scale=stbtt_ScaleForPixelHeight(&font,FONT_SIZE);
	for (int i=32;i<127;i++){
		int glyph=stbtt_FindGlyphIndex(&font,i);
		if (!glyph){
			sll_array_push(NULL,sll_new_object(SLL_CHAR("c00000Z1"),i),&glyphs);
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
		sll_size_t size=((sll_size_t)width)*height;
		uint8_t* bitmap=sll_allocate_stack(size);
		stbtt_MakeGlyphBitmap(&font,bitmap,width,height,width,scale,scale,glyph);
		sll_array_push(NULL,sll_new_object(SLL_CHAR("cffffflu"),i,x0/FONT_SIZE,y0/FONT_SIZE,width/FONT_SIZE,height/FONT_SIZE,advance*scale/FONT_SIZE,bitmap,size,(width?width:1)),&glyphs);
		sll_deallocate(bitmap);
	}
	sll_new_object_array(SLL_CHAR("fffa"),out,ascent*scale/FONT_SIZE,descent*scale/FONT_SIZE,line_gap*scale/FONT_SIZE,&glyphs);
}

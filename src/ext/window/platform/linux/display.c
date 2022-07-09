#include <sll.h>
#include <stdlib.h>
#include <window/common.h>
#include <window/display.h>
#include <window/platform/linux.h>
#include <xcb/randr.h>
#include <xcb/xcb.h>



__WINDOW_API_CALL void window_api_display_enumerate(sll_array_t* out){
	xcb_randr_get_screen_resources_current_reply_t* reply=xcb_randr_get_screen_resources_current_reply(
			_xcb_conn,xcb_randr_get_screen_resources_current(_xcb_conn,_xcb_screen->root),NULL);
	int count=xcb_randr_get_screen_resources_current_outputs_length(reply);
	sll_array_create(count,out);
	xcb_randr_output_t* data=xcb_randr_get_screen_resources_current_outputs(reply);
	xcb_randr_get_output_primary_reply_t* primary_reply=xcb_randr_get_output_primary_reply(_xcb_conn,xcb_randr_get_output_primary(_xcb_conn,_xcb_screen->root),NULL);
	xcb_randr_output_t primary_id=primary_reply->output;
	free(primary_reply);
	sll_array_length_t i=0;
	for (int j=0;j<count;j++){
		xcb_randr_output_t id=*(data+j);
		xcb_randr_get_output_info_reply_t* output=xcb_randr_get_output_info_reply(_xcb_conn,xcb_randr_get_output_info(_xcb_conn,id,reply->config_timestamp),NULL);
		if (!output||output->crtc==XCB_NONE){
			free(output);
			continue;
		}
		xcb_randr_get_crtc_info_reply_t* crtc=xcb_randr_get_crtc_info_reply(_xcb_conn,xcb_randr_get_crtc_info(_xcb_conn,output->crtc,reply->config_timestamp),NULL);
		sll_float_t rotation=0;
		if (crtc->rotation==XCB_RANDR_ROTATION_ROTATE_90){
			rotation=90;
		}
		else if (crtc->rotation==XCB_RANDR_ROTATION_ROTATE_180){
			rotation=180;
		}
		else if (crtc->rotation==XCB_RANDR_ROTATION_ROTATE_270){
			rotation=270;
		}
		sll_integer_t flags=0;
		if (output->connection==XCB_RANDR_CONNECTION_CONNECTED){
			flags|=WINDOW_DISPLAY_FLAG_ACTIVE;
		}
		if (id==primary_id){
			flags|=WINDOW_DISPLAY_FLAG_MAIN_DISPLAY;
		}
		out->data[i]=sll_new_object(SLL_CHAR("u[hhuu]fi"),id,crtc->x,crtc->y,crtc->width,crtc->height,rotation,flags);
		i++;
		free(crtc);
		free(output);
	}
	sll_allocator_resize((void**)(&(out->data)),i*sizeof(sll_object_t*));
	out->length=i;
	free(reply);
}

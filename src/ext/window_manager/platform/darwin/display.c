#include <ApplicationServices/ApplicationServices.h>
#include <sll.h>
#include <stdint.h>
#include <window_manager/common.h>
#include <window_manager/display.h>



__WINDOW_MANAGER_API_CALL void window_api_display_enumerate(sll_array_t* out){
	uint32_t count;
	if (CGGetOnlineDisplayList(0,NULL,&count)!=kCGErrorSuccess){
		SLL_INIT_ARRAY(out);
		return;
	}
	CGDirectDisplayID* data=sll_allocate_stack(count*sizeof(CGDirectDisplayID));
	if (CGGetOnlineDisplayList(count,data,&count)!=kCGErrorSuccess){
		sll_deallocate(data);
		SLL_INIT_ARRAY(out);
		return;
	}
	CGDirectDisplayID main_id=CGMainDisplayID();
	sll_array_create(count,out);
	for (uint32_t i=0;i<count;i++){
		CGDirectDisplayID id=*(data+i);
		CGRect rect=CGRectStandardize(CGDisplayBounds(id));
		sll_integer_t flags=0;
		if (CGDisplayIsActive(id)){
			flags|=WINDOW_DISPLAY_FLAG_ACTIVE;
		}
		if (id==main_id){
			flags|=WINDOW_DISPLAY_FLAG_MAIN_DISPLAY;
		}
		out->data[i]=sll_new_object(SLL_CHAR("u[hhuu]fi"),id,(int32_t)(rect.origin.x),(int32_t)(rect.origin.y),(uint32_t)(rect.size.width),(uint32_t)(rect.size.height),CGDisplayRotation(id),flags);
	}
	sll_deallocate(data);
}

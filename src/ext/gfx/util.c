#include <gfx/context.h>
#include <gfx/util.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



uint32_t _get_memory_type(const gfx_context_data_t* ctx,uint32_t mask,VkMemoryPropertyFlagBits properties){
	for (uint32_t i=0;i<ctx->device.memory_properties.memoryTypeCount;i++){
		if ((mask&(1<<i))&&((ctx->device.memory_properties.memoryTypes+i)->propertyFlags&properties)==properties){
			return i;
		}
	}
	SLL_WARN("No memory type found!");
	return 0;
}



VkFormat _encode_data_format(gfx_data_format_t data_format){
	data_format&=GFX_DATA_FORMAT_SIZE_MASK|GFX_DATA_FORMAT_COUNT_MASK|GFX_DATA_FORMAT_TYPE_MASK|GFX_DATA_FORMAT_FLAG_BGR;
	if ((data_format&(GFX_DATA_FORMAT_SIZE_MASK|GFX_DATA_FORMAT_TYPE_MASK))==(GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_TYPE_FLOAT)){
		SLL_WARN("Invalid combination of data type and size: float8");
	}
	else if ((data_format&GFX_DATA_FORMAT_TYPE_MASK)==GFX_DATA_FORMAT_TYPE_COLOR_UNORM&&(data_format&GFX_DATA_FORMAT_SIZE_MASK)!=GFX_DATA_FORMAT_SIZE_8BIT){
		SLL_WARN("Invalid combination of data type and size: unorm%u",(1<<(data_format&GFX_DATA_FORMAT_SIZE_MASK))<<3);
	}
	else if ((data_format&GFX_DATA_FORMAT_TYPE_MASK)==GFX_DATA_FORMAT_TYPE_COLOR_SNORM&&(data_format&GFX_DATA_FORMAT_SIZE_MASK)!=GFX_DATA_FORMAT_SIZE_8BIT){
		SLL_WARN("Invalid combination of data type and size: snorm%u",(1<<(data_format&GFX_DATA_FORMAT_SIZE_MASK))<<3);
	}
	else if ((data_format&GFX_DATA_FORMAT_TYPE_MASK)==GFX_DATA_FORMAT_TYPE_COLOR_SRGB&&(data_format&GFX_DATA_FORMAT_SIZE_MASK)!=GFX_DATA_FORMAT_SIZE_8BIT){
		SLL_WARN("Invalid combination of data type and size: srgb%u",(1<<(data_format&GFX_DATA_FORMAT_SIZE_MASK))<<3);
	}
	switch (data_format){
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R8_UINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R8_SINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_COLOR_UNORM:
			return VK_FORMAT_R8_UNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_COLOR_SNORM:
			return VK_FORMAT_R8_SNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_COLOR_SRGB:
			return VK_FORMAT_R8_SRGB;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R8G8_UINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R8G8_SINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_COLOR_UNORM:
			return VK_FORMAT_R8G8_UNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_COLOR_SNORM:
			return VK_FORMAT_R8G8_SNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_COLOR_SRGB:
			return VK_FORMAT_R8G8_SRGB;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R8G8B8_UINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R8G8B8_SINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_COLOR_UNORM:
			return VK_FORMAT_R8G8B8_UNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_COLOR_SNORM:
			return VK_FORMAT_R8G8B8_SNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_COLOR_SRGB:
			return VK_FORMAT_R8G8B8_SRGB;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_UINT|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8_UINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_SINT|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8_SINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_COLOR_UNORM|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8_UNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_COLOR_SNORM|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8_SNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_COLOR_SRGB|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8_SRGB;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R8G8B8A8_UINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R8G8B8A8_SINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_COLOR_UNORM:
			return VK_FORMAT_R8G8B8A8_UNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_COLOR_SNORM:
			return VK_FORMAT_R8G8B8A8_SNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_COLOR_SRGB:
			return VK_FORMAT_R8G8B8A8_SRGB;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_UINT|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8A8_UINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_SINT|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8A8_SINT;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_COLOR_UNORM|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8A8_UNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_COLOR_SNORM|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8A8_SNORM;
		case GFX_DATA_FORMAT_SIZE_8BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_COLOR_SRGB|GFX_DATA_FORMAT_FLAG_BGR:
			return VK_FORMAT_B8G8R8A8_SRGB;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R16_UINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R16_SINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R16_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R16G16_UINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R16G16_SINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R16G16_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R16G16B16_UINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R16G16B16_SINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R16G16B16_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R16G16B16A16_UINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R16G16B16A16_SINT;
		case GFX_DATA_FORMAT_SIZE_16BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R32_UINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R32_SINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R32_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R32G32_UINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R32G32_SINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R32G32_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R32G32B32_UINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R32G32B32_SINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R32G32B32_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R32G32B32A32_UINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R32G32B32A32_SINT;
		case GFX_DATA_FORMAT_SIZE_32BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R64_UINT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R64_SINT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_1|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R64_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R64G64_UINT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R64G64_SINT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_2|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R64G64_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R64G64B64_UINT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R64G64B64_SINT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_3|GFX_DATA_FORMAT_TYPE_FLOAT:
			return VK_FORMAT_R64G64B64_SFLOAT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_UINT:
			return VK_FORMAT_R64G64B64A64_UINT;
		case GFX_DATA_FORMAT_SIZE_64BIT|GFX_DATA_FORMAT_COUNT_4|GFX_DATA_FORMAT_TYPE_SINT:
			return VK_FORMAT_R64G64B64A64_SINT;
	}
	return VK_FORMAT_R64G64B64A64_SFLOAT;
}



VkShaderStageFlags _encode_shader_stages(gfx_shader_stage_t stage){
	VkShaderStageFlags out=0;
	if (stage&GFX_SHADER_STAGE_VERTEX){
		out|=VK_SHADER_STAGE_VERTEX_BIT;
	}
	if (stage&GFX_SHADER_STAGE_TESSELLATION_CONTROL){
		out|=VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	}
	if (stage&GFX_SHADER_STAGE_TESSELLATION_EVALUATION){
		out|=VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	}
	if (stage&GFX_SHADER_STAGE_GEOMETRY){
		out|=VK_SHADER_STAGE_GEOMETRY_BIT;
	}
	if (stage&GFX_SHADER_STAGE_FRAGMENT){
		out|=VK_SHADER_STAGE_FRAGMENT_BIT;
	}
	return out;
}

#version 450 core



layout (location=0) in vec3 in_data;
layout (location=0) out vec4 out_color;
layout (binding=1) uniform sampler2D texture_sampler[4096];
layout (push_constant) uniform texture_index_buffer{
	uint texture_index;
};



void main(void){
	out_color=(texture_index==0xffffffff?vec4(in_data,1.0):texture(texture_sampler[texture_index],in_data.xy));
}

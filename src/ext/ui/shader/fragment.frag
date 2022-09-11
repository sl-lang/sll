#version 450 core



layout (location=0) in vec3 in_data;
layout (location=0) out vec4 out_color;
layout (binding=1) uniform sampler2D texture_sampler[4096];



void main(void){
	out_color=(in_data.z>=0?vec4(in_data,1.0):texture(texture_sampler[uint(-in_data.z)-1],in_data.xy));
}

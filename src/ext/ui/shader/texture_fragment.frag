#version 450



layout (location=0) in vec2 in_coord;
layout (location=0) out vec4 out_color;
layout (binding=0) uniform sampler2D texture_sampler;



void main(void){
	out_color=texture(texture_sampler,in_coord);
}

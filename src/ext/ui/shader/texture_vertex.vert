#version 450



layout (location=0) in vec3 in_pos;
layout (location=1) in vec2 in_coord;
layout (location=0) out vec2 out_coord;
layout (binding=0) uniform transform_buffer{
	mat4 transform;
};



void main(void){
	gl_Position=vec4(in_pos,1.0)*transform;
	out_coord=in_coord;
}

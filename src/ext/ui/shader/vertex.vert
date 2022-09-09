#version 450 core



layout (location=0) in vec3 in_pos;
layout (location=1) in vec3 in_data;
layout (location=0) out vec3 out_data;
layout (binding=0) uniform transform_buffer{
	mat4 transform;
};



void main(void){
	gl_Position=vec4(in_pos,1.0)*transform;
	out_data=in_data;
}

#version 450 core



layout (location=0) in vec3 in_data;
layout (location=0) out vec4 out_color;
layout (binding=1) uniform sampler2D texture_sampler[4096];



void main(void){
	if (in_data.z>=0){
		out_color=vec4(in_data,1.0);
	}
	else{
		uint id=uint(-in_data.z-1);
		out_color=texture(texture_sampler[id],in_data.xy);
		if (id==0){
			out_color=vec4(1.0,1.0,1.0,out_color.r);
		}
	}
}

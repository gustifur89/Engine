#version 430 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

void main(){
	gl_Position.xy = pos.xy; //vec4(pos,1);
	gl_Position.z = 1.0;
	gl_Position.w = 1.0;
	UV = (pos.xy+vec2(1,1))/2.0;
}


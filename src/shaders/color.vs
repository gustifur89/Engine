#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 col;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 NM;

out vec3 frg_color;
out vec3 frg_norm;
out vec3 frg_pos;
out float w;
out float z;

void main () 
{
	vec4 tempPos = MVP * vec4(pos, 1);
	gl_Position = tempPos;
	frg_color = col;
	//NM needs to send them to View space
	frg_norm = (NM * vec4(norm, 0)).xyz;
	frg_pos = vec3(NM * vec4(pos,1));
	z = tempPos.z;
	w = tempPos.w;
}
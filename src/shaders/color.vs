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
out float depth;

void main () 
{
	gl_Position = MVP * vec4(pos, 1);
	frg_color = col;	
	frg_norm = (NM * vec4(norm, 0)).xyz;
	frg_pos = (NM * vec4(pos,1)).xyz;
	depth = (gl_Position.z - 0.1) / (100.0 - 0.1);//(gl_Position.z / gl_Position.w);
}
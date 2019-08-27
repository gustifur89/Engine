#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 UV;

uniform mat4 MVP;

out vec2 frg_UV;
out vec3 frg_norm;
out vec3 frg_pos;
out float depth;

void main () 
{
	gl_Position = MVP * vec4(pos, 1);
	frg_UV = UV;
	frg_norm = norm;
	frg_pos = pos;
	depth = (gl_Position.z - 0.1) / (100.0 - 0.1);
}
#version 430 core

layout (points) in;
layout (points, max_vertices = 1) out;

in vec2 UV [];
out vec2 uv;

void main() {    
    gl_Position = gl_in[0].gl_Position; 
	uv = UV[0];
    EmitVertex();
    EndPrimitive();






}  



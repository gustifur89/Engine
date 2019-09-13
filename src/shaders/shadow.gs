#version 430 core

layout (points) in;
layout (points, max_vertices = 1) out;

//struct Light {
//	vec3 pos;
//	float radius;
//	float intensity;
//	mat4 projectionViewMatrix;
//	float padding;
//};
//
//layout(std430, binding = 2) buffer LightSSBO { Light lights[]; };


void main() {    
//Lights[0].projectionMatrix *
    gl_Position =  Lights[0].projectionMatrix  * gl_in[0].gl_Position; 
    EmitVertex();
    EndPrimitive();
}  



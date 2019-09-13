#version 330 core

layout (location = 0) in vec3 pos;


uniform mat4 MV;
uniform mat4 lvm;

out float depth;

//out vec3 frg_pos;

//
//struct Light {
//	vec3 pos;
//	float radius;
//	float intensity;
//	mat4 projectionViewMatrix;
//	float padding;
//};
//
//layout(std430, binding = 2) buffer LightSSBO { Light lights[]; };

void main () 
{
	//gl_Position = lights[0].projectionViewMatrix * MV * vec4(pos, 1);
	//frg_pos = pos;

	gl_Position = lvm * MV * vec4(pos, 1);

	depth = (gl_Position.z - 0.1) / (30.0 - 0.1);    //(gl_Position.z / gl_Position.w);
}
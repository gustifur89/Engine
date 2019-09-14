#version 430 core

layout (location = 0) in vec3 pos;


uniform mat4 MV;
uniform mat4 lvm;

out float depth;

//out vec3 frg_pos;


//struct Light {
//	vec3 pos;
//	float pad0;
//	mat4 LightSpaceMatrix;
//	float pad1;
//	float radius;
//	float intensity;
//};
//
//layout(std430, binding = 2) buffer LightSSBO { 
//	Light lights[]; 
//};

void main () 
{

	//gl_Layer = 0;
	gl_Position = MV * vec4(pos, 1);

//	gl_Position = lights[0].LightSpaceMatrix * MV * vec4(pos, 1);

	//frg_pos = pos;

	//gl_Position = lvm * MV * vec4(pos, 1);

//	depth = (gl_Position.z - 0.1) / (60.0 - 0.1);    //(gl_Position.z / gl_Position.w);
}
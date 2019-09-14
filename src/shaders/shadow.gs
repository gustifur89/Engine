#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform int numShadows;


struct Light {
	vec3 pos;
	float pad0;
	mat4 LightSpaceMatrix;
	float pad1;
	float radius;
	float intensity;
	float pad2;
//	vec3 color;
//	float pad3;
};

layout(std430, binding = 2) buffer LightSSBO { 
	Light lights[]; 
};


void main() {    
	int layer, i;
	for(layer = 0; layer < numShadows; layer++)
	{
		gl_Layer = layer;
		for(i=0;i<gl_in.length();i++)
		{
			gl_Position = lights[layer].LightSpaceMatrix * gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	
	}
//gl_Layer = 0;
//for(int i=0;i<3;i++)
//{
//	vec4 FragPos = gl_in[i].gl_Position;
//     gl_Position = lights[0].LightSpaceMatrix * FragPos;
//	EmitVertex();
//}
// EndPrimitive();


	//  gl_Position =  Lights[0].LightSpaceMatrix  * gl_in[0].gl_Position; 
}  


//int layer, i;
//    for(layer=0; layer<4; layer++){
//        gl_Layer = layer;
//        for(i=0; i<gl_in.length(); i++){
//            gl_Position = gl_in[i].gl_Position;
//            EmitVertex();
//        }
//        EndPrimitive();
//    }


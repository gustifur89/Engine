#version 430 core

in vec2 UV;

out vec4 color;

//uniform sampler2D renderedTexture;
uniform sampler2D colTex;
uniform sampler2D posTex;
uniform sampler2D normTex;
uniform sampler2DArray shadowMap; // sampler2DArray 


uniform int numShadows;


uniform float gamma;
uniform float exposure;

uniform mat4 lsm;


#define LIGHT vec3(1, 1, 0)

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

//std430
layout(std430, binding = 2) buffer LightSSBO { 
	Light lights[]; 
};


float LinearizeDepth(float depth)
{
	float near_plane = 0.1;
	float far_plane = 60.0;
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}


float ShadowCalculation(vec4 fragPosLightSpace, float bias, int indx)
{
  
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  
    projCoords = projCoords * 0.5 + 0.5;
  
   float closestDepth = texture(shadowMap, vec3(projCoords.xy, indx)).r; 
  
    float currentDepth = projCoords.z;
 
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	float adjust = (projCoords.z > 1.0 || projCoords.z < 0.0) ? 0.0 : 1.0;
    return (1 - shadow) * adjust;
}  



float lighting(vec3 pos, vec3 norm, int indx)
{
	vec3 light_pos = lights[indx].pos;
	float light_radius =  lights[indx].radius;
	float light_intensity = lights[indx].intensity;
	mat4 light_spaceMatrix = lights[indx].LightSpaceMatrix;


	vec3 dP = light_pos - pos.xyz;
	float diffuse = clamp(dot(normalize(dP), norm), 0, 1);

	float relDistance = clamp((light_radius - length(dP)) / light_radius, 0, 1);

	float iten = relDistance;

	vec4 posInLS = light_spaceMatrix * vec4(pos.xyz, 1);

	float bias = max(0.000005 * (1.0 - dot(norm, normalize(dP))), 0.000001);

//	float shadow = ShadowCalculation(UV, 0, pos.xyz);       
	float shadow = ShadowCalculation(posInLS, bias, indx);    

	return shadow * diffuse * iten;
}




void main()
{
	vec3 norm = texture( normTex, UV ).xyz;
	vec4 pos = texture( posTex, UV );
	vec4 col = texture( colTex, UV );
	float ambient = 0.1;

	float lightVal = ambient;

	for(int i=0;i<numShadows;i++)
	{
		 lightVal += lighting(pos.xyz, norm, i);
	}

	//lightVal += lighting(pos.xyz, norm, 4);
    color.rgb = col.rgb * lightVal;
	color.a = 1.0;//col.a;
	
	//float val = (texture(shadowMap,UV).r);
	//color.rgb = vec3(val);

	//float val = LinearizeDepth(texture(shadowMap, vec3(UV, 0)).r); 
	//color.rgb = vec3(val);

}
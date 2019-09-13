#version 430 core

in vec2 UV;

out vec4 color;

//uniform sampler2D renderedTexture;
uniform sampler2D colTex;
uniform sampler2D posTex;
uniform sampler2D normTex;
uniform sampler2D shadowMap;//[];

uniform int numShadows = 0;


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
};

//std430
layout(std430, binding = 2) buffer LightSSBO { 
	Light lights[]; 
};

float ShadowCalculation(vec4 fragPosLightSpace, float dif)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	
	float bias = 0.0; //max(0.05 * (1.0 - dif), 0.005);  
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	float adjust = (projCoords.z > 1.0) ? 0.0 : 1.0;
    return shadow * adjust;
}  

//float ShadowCalculation(vec2 uv, int idx, vec3 pos)
//{
//	Light lgt = lights[idx];
//	mat4 pvm = lgt.projectionViewMatrix;
//
//	vec4 fragPosLightSpace = lsm * vec4(pos, 1);
//
////vec3 projCoords = pos_ls.xyz / pos_ls.w;
//////projCoords = projCoords * 0.5 + 0.5;
////
////float shadowDepth = texture(shadowMap, projCoords.xy + vec2(0.5)).r; 
////
//////float depth = (projCoords.z - 0.1) / (100.0 - 0.1);
////float currentDepth = projCoords.z;
////
////return currentDepth > shadowDepth  ? 1.0 : 0.0;
//
//
//
//	// perform perspective divide
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
//    float closestDepth = texture(shadowMap, projCoords.xy).r; 
//    // get depth of current fragment from light's perspective
//    float currentDepth = projCoords.z;
//	//float depth = (projCoords.z - 0.1) / (100.0 - 0.1); 
//
//    // check whether current frag pos is in shadow
//    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
//
//    return shadow;
//}



float LinearizeDepth(float depth)
{
	float near_plane = 0.1;
	float far_plane = 5.0;
    float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
	//calculate lighting and shadows for each pixel
	vec3 norm = texture( normTex, UV ).xyz;
	vec4 pos = texture( posTex, UV );
	vec4 col = texture( colTex, UV );
	//col = texture(shadowMap, vec3(UV,0));

	vec3 light_pos = lights[0].pos;
	float light_radius =  lights[0].radius;
	float light_intensity = lights[0].intensity;
	mat4 light_spaceMatrix = lights[0].LightSpaceMatrix;


	vec3 dP = light_pos - pos.xyz;
	float diffuse = clamp(dot(normalize(dP), norm), 0, 1);

	float relDistance = clamp((light_radius - length(dP)) / light_radius, 0, 1);

	float iten = relDistance;//clamp(light_intensity * ((light_radius - length(dP)) / light_radius), 0, 1);
	float ambient = 0.1;

//	vec4 posInLS = lsm * vec4(pos.xyz, 1);
	vec4 posInLS = light_spaceMatrix * vec4(pos.xyz, 1);

//	float shadow = ShadowCalculation(UV, 0, pos.xyz);       
	float shadow = ShadowCalculation(posInLS, dot(normalize(dP), norm));    
	//shadow = 1.0;

//	vec3 dP = lights[i].pos - pos.xyz;
//	float diff = clamp(dot(normalize(dP), norm), 0, 1);//clamp(dot(normalize(dP), normalize(norm)), 0, 1);
//	float inten = ((10 - length(dP)) / 10);//((length(dP) - lights[i].radius) / lights[i].radius);

	float lighting = ambient + (1 - ambient) * shadow * diffuse * iten;

    color.rgb = col.rgb * lighting;// = (ambient + (1.0 - shadow) * (diffuse + specular)) * col; 
	color.a = 1.0;//col.a;
	

	//float val = (texture(shadowMap,UV).r);
	//color.rgb = vec3(val);

//	float sky = norm.xyz == vec3(0.0) ? 1.0 : 0.0;

//	vec4 fragPosLightSpace = lsm * vec4(pos.xyz, 1);
//	color.rgb = fragPosLightSpace.rgb / 10.0;


//    
//	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    
//	projCoords = projCoords * 0.5 + 0.5;
//
//	float val = (texture(shadowMap,UV).r);// projCoords.xy).r; 
//
//
//	color.rgb = vec3(val);//vec3(UV, 1.0) * (1 - sky) + col.rgb * sky;

	//color.rgb = vec3(projCoords.xy, 1.0);
	//color.rgb = vec3(shadow);

//	color.rgb = vec3(LinearizeDepth(projCoords.z));

//	vec3 lightPos = vec3(0,6,0);
//
//	//color = vec4(texture(textureArray, vec3(TexCoords.xy, layer)));
//	float sValue = 0.0;
//	for(int i = 0; i < numShadows; i++)
//	{
//		//color = vec4(texture(textureArray, vec3(TexCoords.xy, layer)));
//
//
//
//
//
//		vec3 dP = lights[i].pos - pos.xyz;
//		float diff = clamp(dot(normalize(dP), norm), 0, 1);//clamp(dot(normalize(dP), normalize(norm)), 0, 1);
//		float inten = ((10 - length(dP)) / 10);//((length(dP) - lights[i].radius) / lights[i].radius);
//
//		sValue += diff;//inten;//(length(vec3(pos) - lights[i].pos)/2.0) * diff;
//	}
//
//	float lightValue = 1.0;//;//1.0;
//
//	//float diffuse = clamp(dot(normalize(LIGHT), norm), 0, 1);
//	//float diffuse_ramp = 1.0 * float(diffuse >= 0.4) + 0.4 * float(diffuse < 0.4);
//	//lightValue = diffuse * 0.4 + 0.6;
//	//lightValue += shadowValue;
//
//	//correct it with the gamma
//	float mapped = 1.0 - exp(-lightValue * exposure);
//	mapped = pow(mapped, 1.0 / gamma);
//	
//	color = lightValue * col * (1 - pos.w);//lightValue * col; //mapped
//	//color.rgb = vec3(texture(shadowMap[0], UV).r);
	
//	color.rgb = vec3(texture(shadowMap, UV).r);
}
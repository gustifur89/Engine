#version 430 core

in vec2 UV;

out vec4 color;

//uniform sampler2D renderedTexture;
uniform sampler2D colTex;
uniform sampler2D posTex;
uniform sampler2D normTex;
uniform sampler2DArray shadowMap; // sampler2DArray 
uniform sampler2D ssaoTex;

uniform int numShadows;

uniform vec2 windowScale;
uniform float gamma;
uniform float exposure;
uniform vec3 globalLightDir;
uniform float globalLightInten;
uniform float ambient;
uniform int useShadow;
uniform int useSSAO;
uniform mat4 lsm;
uniform mat4 VSMat;
uniform vec2 sgfirohti;
uniform mat3 conMatX;
uniform mat3 conMatY;


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
	vec3 light_pos = (VSMat * vec4(lights[indx].pos, 1)).xyz;//view space
	float light_radius =  lights[indx].radius;				
	float light_intensity = lights[indx].intensity;			
	mat4 light_spaceMatrix = lights[indx].LightSpaceMatrix;	//View to Light space


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

float globalLighting(vec3 norm)
{
	float diffuse = clamp(dot((VSMat * vec4(globalLightDir,0)).xyz, norm), 0, 1);
	return diffuse * globalLightInten;
}

float getAmbient()
{
//texture( ssaoTex, UV ).x; // 0.6
//	vec2 texelSize = vec2(1/800,1/600);   //1.0 / vec2(textureSize(ssaoInput, 0));
//    float result = 0.0;
//    for (int x = -2; x < 2; ++x) 
//    {
//        for (int y = -2; y < 2; ++y) 
//        {
//            vec2 offset = vec2(float(x), float(y)) * texelSize;
//            result += texture(ssaoTex, UV + offset).r;
//        }
//    }
 //  return result / (4.0 * 4.0);

//	float val = (useSSAO == 1 ? texture(ssaoTex, UV).r  : 1.0); 
	return texture(ssaoTex, UV).r;
}


void main()
{
	vec3 norm = texture( normTex, UV ).xyz;
	vec4 pos = texture( posTex, UV );
	vec4 col = texture( colTex, UV );
	float lightVal = ambient;

#ifdef USE_SSAO
	lightVal *= getAmbient();
#endif	
	
	lightVal += globalLighting(norm);

#ifdef USE_SHADOW
	for(int i=0;i<numShadows;i++)
	{
		lightVal += lighting(pos.xyz, norm, i);
	}
#endif

	//lightVal += lighting(pos.xyz, norm, 4);
    color.rgb = col.rgb * (col.a == 0 ? lightVal : 1.0);
	
#ifdef USE_SOBEL
	vec3 sumX = vec3(0.0);
	vec3 sumY = vec3(0.0);
    vec3 tempCol = vec3(0.0);
    for (int x = -1 ; x <= 1 ; x++)
    {
        for (int y = -1 ; y <= 1 ; y++)
        {
            //soDep = LinearizeDepth(texture( posTex, UV + vec2(windowScale.x * x, windowScale.y * y)).z);
			tempCol = texture( colTex, UV + vec2(windowScale.x * x, windowScale.y * y)).rgb;
            sumX += conMatX[x+1][y+1] * tempCol;// * conMatX[x+1][y+1];
            sumY += conMatY[x+1][y+1] * tempCol;// * conMatY[x+1][y+1];
	    }
    }
   //float val = sqrt(sumX * sumX + sumY * sumY);
   vec3 sum = sqrt(sumX * sumX + sumY * sumY);
   //float choice = float(val > 0.01);
   float choice = float(length(sum) > 0.1);
   color.rgb = choice * vec3(0.0,0.0,0.0) + (1.0-choice) * color.rgb;
   //color.rgb = vec3(0.5 * soDep + 0.5);
#endif
	

	//color.rgb = pos.xyz;

	color.a = 1.0;
	
//	color.rgb = vec3(texture( ssaoTex, UV ).x);

	//float val = (texture(shadowMap,UV).r);
	//color.rgb = vec3(val);

	//float val = LinearizeDepth(texture(shadowMap, vec3(UV, 0)).r); 
	//color.rgb = vec3(val);

}
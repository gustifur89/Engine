#version 430 core

in vec2 UV;

out float ao;

uniform sampler2D posTex;
uniform sampler2D normTex;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;
uniform mat4 viewMatrix;

uniform vec2 noiseScale;

const int kernelSize = 32;
const float radius = 1.0;//1.5
const float bias = 0.06;

void main()
{
	vec3 fragPos   = texture(posTex, UV).xyz;//(viewMatrix * vec4(texture(posTex, UV).xyz,1)).xyz;
	vec3 normal    = texture(normTex, UV).xyz;//(viewMatrix * vec4(texture(normTex, UV).xyz, 0)).xyz;
	vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  

	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal);  
	
	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; ++i)
	{
	    // get sample position
	    vec3 samp = TBN * samples[i]; // From tangent to view-space
	    samp = fragPos + samp * radius; 
	    	   
		vec4 offset = vec4(samp, 1.0);
		offset      = projection * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0  

		
		float sampleDepth = texture(posTex, offset.xy).z; //(viewMatrix * vec4(texture(posTex, offset.xy).xyz, 1.0)).z; 

		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= samp.z + bias ? 1.0 : 0.0) * rangeCheck;   
	}  
	
	occlusion = 1.0 - (occlusion / kernelSize);
	ao = occlusion;  

}


//float ndc_bias(float bias)
//{
//	//return (((100.0-0.1) * depth) + 0.1 + 100.0) / 2.0;
//	return (2.0 * bias - 0.1 - 100.0) / ( 100.0 - 0.1 );
//}
//
//
//void main()
//{
//	vec3 normal = normalize(texture( normTex, UV ).xyz);
//	vec4 pos_ = texture( posTex, UV );
//	vec3 pos = pos_.xyz;//(viewMatrix * vec4(pos_.xyz, 1)).xyz;
//	
//	vec3 randomVec = texture(texNoise, UV * noiseScale).xyz;  
//
//	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
//	vec3 bitangent = cross(normal, tangent);
//	mat3 TBN       = mat3(tangent, bitangent, normal);  
//
//
//	int kernelSize = 64;
//	float radius = 0.5;
//	//float bias = -0.01;//0.001;//-0.01;
//
//
//	float sampleDepthHere = 0.0;
//
//	vec4 cameraPoint = viewMatrix[3];
//
//	float occlusion = 0.0;
//	for(int i = 0; i < kernelSize; i++)
//	{
//	
//		// get sample position
//	//	vec3 thingy = TBN * samples[i]; // From tangent to view-space
//	//	thingy = pos + thingy * radius;  //From View to... something... Pos in viewSpace. 
//    //
//	//	vec4 offset = vec4(thingy, 1.0);
//	//	offset      = projection * offset;    // from view to clip-space
//	//	offset.xyz /= offset.w;               // perspective divide
//	//	offset.xy  = offset.xy * 0.5 + 0.5; // transform to range 0.0 - 1.0  
//	//
//	//	float sampleDepth = texture(posTex, offset.xy).w; 
//	////	float rangeCheck = smoothstep(0.0, 1.0, radius / abs(pos.z - sampleDepth));
//	////	occlusion += (sampleDepth >= thingy.z + bias ? 1.0 : 0.0) * rangeCheck;   
//	//	occlusion += (sampleDepth < offset.z + bias ? 1.0 : 0.0);  
//
//
//	//	vec3 thingy = pos + radius * samples[i]; // From tangent to view-space
//	//	vec4 offset = vec4(thingy, 1.0);
//	//	offset      = projection * viewMatrix * offset;    // from world to clip-space
//	//	offset.xyz /= offset.w;               // perspective divide
//	//	offset.xy  = offset.xy * 0.5 + 0.5; // transform to range 0.0 - 1.0  
//	//	vec4 point = projection * viewMatrix * texture(posTex, offset.xy); // from world to clip-space
//	//	point.z /= point.w; 
//	//	
//	////	float sampleDepth = texture(posTex, offset.xy).w; 
//	//
//	//	//float rangeCheck = smoothstep(0.0, 1.0, radius / abs(pos.z - sampleDepth));
//	//	//occlusion += (offset.z + bias >= point.z ? 1.0 : 0.0) * rangeCheck;   
//	//	occlusion += (offset.z >= point.z + bias ? 1.0 : 0.0);  
//
//		vec3 testPoint = pos + radius * samples[i];			//get the test point in the sphere around the current point.
//															//	World space
//		vec4 tpInCS = projection * viewMatrix * vec4(testPoint, 1.0);	//Transform to clip-space
//		tpInCS /= tpInCS.w;
//		tpInCS.xy = tpInCS.xy * 0.5 + 0.5;						//Normalize from [0,1]
//
//		vec3 depthPoint = texture(posTex, tpInCS.xy).xyz;	//get the real point in that location in view space
//		vec4 dpInCS = projection * viewMatrix * vec4(depthPoint, 1.0);	//Transform to clip-space
//		dpInCS /= dpInCS.w;
//		//dpInCS = dpInCS * 0.5 + 0.5;						//Normalize from [0,1]
//		vec3 normTP = texture(normTex, tpInCS.xy).xyz;
//		vec3 dP = depthPoint - cameraPoint.xyz;
//
//
//		float bias = -max(0.0005 * (1.0 - dot(normTP, normalize(dP))), 0.001);
//
//		//check if the depth is greater => occluded
//		occlusion += (tpInCS.z > dpInCS.z + bias * ( 1 - dpInCS.z) ? 1.0 : 0.0); //  ndc_bias(bias)
//	} 
//	
//	occlusion = 1.0 - (occlusion / kernelSize);
//
//	ao = occlusion;//.wocclusion;
//}
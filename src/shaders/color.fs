#version 330 core

in vec3 frg_color;
in vec3 frg_norm;
in vec3 frg_pos;
in float w;
in float z;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 pos;
layout(location = 2) out vec3 norm;



//uniform mat4 MVP;
//uniform mat4 mv;
uniform mat4 NM;
uniform mat4 ColorMatrix;
uniform vec3 light;

#define LIGHT vec3(0, 1, 0)

void main()
{

	vec3 normInSpace = normalize(NM * vec4(frg_norm, 0.0)).xyz;
	
	color = vec4((ColorMatrix * vec4(frg_color, 1.0)).rgb, 0.0);
	pos = vec4(frg_pos, z / w); //vec4((NM * vec4(frg_pos, 1.0)).xyz, depth); depth
	norm = frg_norm;//normInSpace;
	
//	vec3 normInSpace = normalize(NM * vec4(frg_norm, 0.0)).xyz;
//	
//	float diffuse = clamp(dot(light, normInSpace), 0, 1);
//	float diffuse_ramp = 1.0 * float(diffuse >= 0.4) + 0.4 * float(diffuse < 0.4);
//	
//	float color_value = diffuse * 0.4 + 0.6;
//
//
//	//0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y))
//	
//	vec4 pos = NM * vec4(frg_pos, 1.0);
//
//	pos = floor(pos * 10000) / 10000;
//
//	float random = float(fract(sin(dot(pos.xyz ,vec3(12.9898,78.233,36.548))) * 43758.5453) > 0.98) * -0.2 + 1.0; // vec3(sin(pos.x*100), cos(pos.y*100), sin(pos.z*100)) * 0.2;
//
//	
//	color.rgb = (ColorMatrix * vec4(frg_color, 1.0) * color_value * random).rgb;
//	
//	
//	color.a = depth;
//
//	pos = vec4(frg_pos, depth);
//	norm = normInSpace;
	
}
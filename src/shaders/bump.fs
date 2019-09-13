#version 330 core

in vec3 frg_color;
in vec3 frg_norm;
in vec3 frg_pos;
in float depth;

layout(location = 0) out vec4 color;
//uniform mat4 MVP;
//uniform mat4 mv;
uniform mat4 NM;
uniform mat4 ColorMatrix;
uniform vec3 light;

#define LIGHT vec3(0, 1, 0)

void main()
{
    vec4 pos = NM * vec4(frg_pos, 1.0);

	pos = floor(pos * 100) / 100;

	float random = 0.0;//float(fract(sin(dot(pos.xyz ,vec3(12.9898,78.233,36.548))) * 43758.5453) > 0.5) * -0.2 + 1.0; 


    float randomX = fract(sin(dot(pos.xyz + vec3(1.0, 0.0, 0.0) ,vec3(12.9898,78.233,36.548))) * 43758.5453) * 0.01;
    float randomY = fract(sin(dot(pos.xyz + vec3(0.0, 1.0, 0.0) ,vec3(12.9898,78.233,36.548))) * 43758.5453) * 0.01;
    float randomZ = fract(sin(dot(pos.xyz + vec3(0.0, 0.0, 1.0) ,vec3(12.9898,78.233,36.548))) * 43758.5453) * 0.01;
    vec3 randomVec = vec3(randomX, randomY, randomZ);


	vec3 normInSpace = normalize(NM * vec4(frg_norm, 0.0)).xyz + randomVec;
	float diffuse = clamp(dot(light, normInSpace), 0, 1);
	float diffuse_ramp = 1.0 * float(diffuse >= 0.4) + 0.4 * float(diffuse < 0.4);
	
	float color_value = diffuse * 0.4 + 0.6;


	
	color.rgb = (ColorMatrix * vec4(frg_color, 1.0) * color_value * random).rgb;
	
	
	color.a = depth;
}
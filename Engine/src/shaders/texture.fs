#version 330 core

in vec2 frg_UV;
in vec3 frg_norm;
in vec3 frg_pos;
in float depth;

layout(location = 0) out vec4 color;

uniform mat4 NM;
uniform vec3 light;
uniform sampler2D tex;

void main()
{
	vec3 normInSpace = normalize(NM * vec4(frg_norm, 0.0)).xyz;
	float diffuse = clamp(dot(light, normInSpace), 0, 1);	
	float color_value = diffuse * 0.4 + 0.6;

	color.rgb = color_value * texture(tex, frg_UV).rgb;
	color.a = depth;
}
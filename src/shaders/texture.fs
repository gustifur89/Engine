#version 330 core

in vec2 frg_UV;
in vec3 frg_norm;
in vec3 frg_pos;
in float w;
in float z;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 pos;
layout(location = 2) out vec3 norm;

uniform sampler2D tex;

void main()
{
	pos = vec4(frg_pos, z / w);
	norm = frg_norm;
	color = vec4(texture(tex, frg_UV).rgb, 0.0);
}

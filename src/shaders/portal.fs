#version 450 core

//in vec3 frg_color;
in vec2 UV;
//in float depth;
//in float w;

in vec3 frg_color;
in vec3 frg_norm;
in vec3 frg_pos;
in float w;
in float z;

uniform sampler2D colTex;
uniform sampler2D posTex;
uniform sampler2D normTex;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 pos;
layout(location = 2) out vec3 norm;

uniform mat4 ColorMatrix;
uniform sampler2D renderedTexture;

void main()
{
//uniform mat4 NM;
//uniform mat4 ColorMatrix;
//  //UV = (UV + vec2(1,1)) /2.0;
// // color.rgb = 0.9 * texture(renderedTexture, clamp((UV+vec2(1,1)) /2.0, 0.0, 1.0) ).rgb + 0.1 * frg_color;
//  vec3 col = (ColorMatrix * vec4(frg_color, 1.0)).rgb;
//  color.rgb = 0.9 * texture(renderedTexture, ((UV/w)+vec2(1,1)) /2.0).rgb + 0.1 * col;
//
//  color.a = texture(renderedTexture, ((UV/w)+vec2(1,1)) /2.0).a;
	//UV = (UV + vec2(1,1)) /2.0;

	vec2 uv = ((UV/w)+vec2(1,1)) /2.0;

	//color.rgb = 0.9 * texture(renderedTexture, clamp((UV+vec2(1,1)) /2.0, 0.0, 1.0) ).rgb + 0.1 * frg_color;
//	vec3 col = (ColorMatrix * vec4(frg_color, 1.0)).rgb;
//	vec4 col_ = texture(renderedTexture, uv);
//	color.rgb = 0.9 * col_.rgb + 0.1 * col;
//	color.a = col_.a;
//	//color = vec4((ColorMatrix * vec4(frg_color, 1.0)).rgb, 0.0);
//	pos = vec4(frg_pos, z / w); //vec4((NM * vec4(frg_pos, 1.0)).xyz, depth); depth
//	norm = frg_norm;

	vec4 col_ = vec4((ColorMatrix * vec4(frg_color, 1.0)).rgb, 0.0);

	color = 0.9 * texture(colTex, uv) + 0.1 * col_;
	pos = texture(posTex, uv);
	norm = texture(normTex, uv).xyz;

}
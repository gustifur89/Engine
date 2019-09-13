#version 330 core

in vec3 frg_color;
in vec2 UV;
in float depth;
in float w;

out vec4 color;

uniform mat4 ColorMatrix;
uniform sampler2D renderedTexture;

void main()
{
    //UV = (UV + vec2(1,1)) /2.0;
   // color.rgb = 0.9 * texture(renderedTexture, clamp((UV+vec2(1,1)) /2.0, 0.0, 1.0) ).rgb + 0.1 * frg_color;
    vec3 col = (ColorMatrix * vec4(frg_color, 1.0)).rgb;
    color.rgb = 0.9 * texture(renderedTexture, ((UV/w)+vec2(1,1)) /2.0).rgb + 0.1 * col;

    color.a = texture(renderedTexture, ((UV/w)+vec2(1,1)) /2.0).a;
}
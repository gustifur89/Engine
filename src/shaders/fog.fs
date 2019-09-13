#version 330 core

in vec2 UV;

out vec3 color;

uniform vec3 clearColor;
uniform sampler2D renderedTexture;

void main()
{
    
    float a = texture(renderedTexture, UV.xy).a;
	color = (1 - a) * texture(renderedTexture, UV.xy).rgb + (a) * clearColor;
}
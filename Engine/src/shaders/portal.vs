#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 col;

uniform mat4 MVP;

out vec2 UV;
out vec3 frg_color;
out float depth;
out float w;


void main(){
	gl_Position = MVP * vec4(pos, 1);
	UV = gl_Position.xy;// / gl_Position.w ;  //((gl_Position.xy / gl_Position.w) + vec2(1,1) ) /2.0;      //clamp(((gl_Position.xy / gl_Position.w) + vec2(1,1) ) /2.0, 0.0, 1.0);
	w = gl_Position.w;
	depth = (gl_Position.z - 0.1) / (100.0 - 0.1);//(gl_Position.z / gl_Position.w);
	frg_color = col;
}


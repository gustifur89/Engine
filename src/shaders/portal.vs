#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec3 col;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 NM;

//out vec2 UV;
//out vec3 frg_color;
//out float depth;
//out float w;

out vec3 frg_color;
out vec3 frg_norm;
out vec3 frg_pos;
out vec2 UV;
out float w;
out float z;

void main(){
	//gl_Position = MVP * vec4(pos, 1);
	//UV = gl_Position.xy;// / gl_Position.w ;  //((gl_Position.xy / gl_Position.w) + vec2(1,1) ) /2.0;      //clamp(((gl_Position.xy / gl_Position.w) + vec2(1,1) ) /2.0, 0.0, 1.0);
	//w = gl_Position.w;
	//depth = (gl_Position.z - 0.1) / (100.0 - 0.1);//(gl_Position.z / gl_Position.w);
	//frg_color = col;

	vec4 tempPos = MVP * vec4(pos, 1);
	gl_Position = tempPos;
	frg_color = col;
	//NM needs to send them to View space
	frg_norm = (NM * vec4(norm, 0)).xyz;
	frg_pos = vec3(NM * vec4(pos,1));
	UV = tempPos.xy;
	w = tempPos.w;
}


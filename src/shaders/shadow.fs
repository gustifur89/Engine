#version 330 core

in float depth;

//in vec3 frg_color;
//in vec3 frg_norm;
//in vec3 frg_pos;
//in float depth;

//layout(location = 5) out float depth_;

//uniform mat4 MVP;
//uniform mat4 mv;
//uniform mat4 NM;
//uniform mat4 ColorMatrix;
//uniform vec3 light;
//
//#define LIGHT vec3(0, 1, 0)

void main()
{
//	vec3 normInSpace = normalize(NM * vec4(frg_norm, 0.0)).xyz;
//	color = vec4((ColorMatrix * vec4(frg_color, 1.0)).rgb, depth);
//	pos = vec4(frg_pos, depth);
//	norm = frg_norm;//normInSpace;	
	//depth_ = 1.0;//depth;
//	deep = 0.5;//gl_FragDepth;
	
	//depth = (gl_Position.z - 0.1) / (100.0 - 0.1);//(gl_Position.z / gl_Position.w);
	//gl_FragDepth =  gl_FragCoord.z;
	
	
//	gl_FragDepth  = (gl_FragCoord.z / gl_FragCoord.w - 0.1) / (100.0 - 0.1);//(gl_Position.z / gl_Position.w);
	gl_FragDepth = depth; 
}
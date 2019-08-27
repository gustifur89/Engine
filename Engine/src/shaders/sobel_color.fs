#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;

uniform mat3 convolutionMatrixX;
uniform mat3 convolutionMatrixY;

const float stepX = 1.0 / 1024.0;
const float stepY = 1.0 / 768.0;

void main()
{
    float sumX = 0.0;
	float sumY = 0.0;
	float temp = 0.0;
    for (int x = -1 ; x <= 1 ; x++)
    {
        for (int y = -1 ; y <= 1 ; y++)
        {
            temp = texture(renderedTexture, vec2(UV.x + x * stepX, UV.y + y * stepY)).a;
            sumX +=  temp * convolutionMatrixX[x+1][y+1];
            sumY += temp * convolutionMatrixY[x+1][y+1];
	    }
    }

//	vec4 sum = sqrt(sumX * sumX + sumY * sumY);
	float choice = float(sqrt(sumX * sumX + sumY * sumY) > 0.1);
	color = texture(renderedTexture, UV.xy).rgb + vec3(choice);//vec4((1.0 - choice) * texture2D(renderedTexture, UV.xy).rgb  + choice * vec3(0.0,0.0,0.0) , 1.0);
//	color = texture2D(renderedTexture, UV.xy).rgb;


}


//#version 330 core

//in vec2 UV;

//out vec4 color;

//uniform sampler2D renderedTexture;

//void main(){
//    color = texture( renderedTexture, UV );// + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).xyz;
//}
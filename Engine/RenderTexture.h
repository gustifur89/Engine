#pragma once
#include "Headers.h"
class RenderTexture
{
public:
	RenderTexture(int width, int height);
	RenderTexture();
	~RenderTexture();

	//parameters
	GLuint frameBuffer;
	GLuint renderedTexture;
	GLuint depthrenderbuffer;
};


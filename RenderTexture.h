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

	GLuint colTex;
	GLuint posTex;
	GLuint normTex;
};

class ShadowTexture
{
public:
	ShadowTexture(int size, int numShadows);
	ShadowTexture();
	~ShadowTexture();
	int size;

	GLuint depthMap;
	GLuint depthMapFBO;
};


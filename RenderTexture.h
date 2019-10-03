#pragma once
#include "Headers.h"
#include "Geometry.h"
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

class SSAOTexture
{
public:
	SSAOTexture(int width, int height);
	SSAOTexture();
	~SSAOTexture();

	//parameters
	GLuint frameBuffer;
	GLuint aoTexture;
	GLuint noiseTexture;
	std::vector<glm::vec3> ssaoKernel;
	std::vector<GLfloat> kernelSamples;
};


#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "RenderTexture.h"
#include "Light.h"

//#define PRINT_FPS

#define UI_SSAO		0b0000001
#define UI_SHADOW	0b0000010
#define UI_SOBEL	0b0000100 //implement
#define UI_COMIC	0b0001000

class UIManager
{
protected:
	double oldTime;
	double oldMouseX, oldMouseY;
	void renderWindow();
	void renderSSAO(Camera& camera);
	void setHDR(float gamma, float exposure);
	std::vector<std::shared_ptr<Light>> cullLights(Camera& camera);
	GLuint clearColorLoc;
	GLuint gammaLoc;
	GLuint exposureLoc;
	GLuint gLDirLoc;
	GLuint gLIntenLoc;
	GLuint ambientLoc;
	GLuint windowScaleLoc;


	GLuint useSSAOLoc;
	GLuint useShadowLoc;
	GLuint colTexLoc;
	GLuint posTexLoc;
	GLuint normTexLoc;
	GLuint shadowTexLoc;
	GLuint ssaoTexLoc;
	GLuint numShadowLoc;
	GLuint VSMatLoc;
	GLuint invVSMatLoc;

	GLuint shadowsSSBOID;
	GLuint shadowMatrixLoc;
	GLuint numShadowSSLoc;

	GLuint ssaoPosTexLoc;
	GLuint ssaoNormTexLoc;
	GLuint ssaoBlurWindowScaleLoc;
	GLuint ssaoBlurAOTexLoc;
	GLuint ssaowindowScaleLoc;
	GLuint samplesLoc;
	GLuint projectionLoc;
	GLuint viewMatrixLoc;
	GLuint noiseScaleLoc;
	GLuint noiseTextureLoc;
	

	GLuint lightLoc;
	GLuint wLSMLoc;

	float gamma;
	float exposure;


	bool useSSAO;
	bool useShadow;
	bool useSobel;
	bool useComic;
	int numShadows;

	glm::vec3 internalLightDirection;

	std::shared_ptr<Texture> comicShadingTexture;

	std::vector<double> frameRates;
	int maxAvg = 10;
	int curFrame = 0;
	

public:
	UIManager();
	~UIManager();

	bool create(int width, int height, std::string title, int fps, unsigned int parameters, std::string postProcessing = "windowFragment");
	void setUpWindowQuad(std::string postProcessing);

	void initializeWindowShaderUniforms();

	void updateWindowShaderUniforms();

	void glSetUp();
	void setClearColor(int r, int g, int b);
	
	void addLight(std::shared_ptr<Light> light);

	bool isWindowOpen();
	bool isKeyPressed(int key);
	bool isMouseDown(int mouseButton);;
	double getMouseX();
	double getMouseY();
	void lockMouse();
	void unlockMouse();
	bool getMouseLockState();
	void setDeltaMouse();
	int setShadowMap(Camera& camera, std::vector<std::shared_ptr<Light>> lights_);

	void delay(double timeDelay);

	void display(Camera& camera, std::shared_ptr<GameObject> renderObject);

	//parameters
	std::shared_ptr<GameObject> stage;
	RenderTexture renderTexture;
	ShadowTexture shadowTexture;
	SSAOTexture ssaoTexture;
	GLFWwindow* window;
	int width, height;
	double aspectRatio;
	double timeDelay;
	int fps;
	bool mouseLockedState;
	double deltaMouseX, deltaMouseY;
	double r, g, b;
	std::string title;
	
	std::shared_ptr<GameObject> HUD;
	std::vector<std::shared_ptr<Light>> lights;
	double deltaTime;
	GLuint windowVBO;
	GLuint windowVAO;
	std::string postProcessingFile;
	GLuint renderTextureID;
	std::shared_ptr <Shader> windowShader;
	std::shared_ptr <Shader> shadowShader;
	std::shared_ptr <Shader> ssaoShader;
	std::shared_ptr <Shader> ssaoBlurShader;
};


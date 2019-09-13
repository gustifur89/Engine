#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "RenderTexture.h"
#include "Light.h"

class UIManager
{
private:
	double oldTime;
	double oldMouseX, oldMouseY;
	void renderWindow();
	void setHDR(float gamma, float exposure);
	GLuint clearColorLoc;
	GLuint gammaLoc;
	GLuint exposureLoc;

	GLuint colTexLoc;
	GLuint posTexLoc;
	GLuint normTexLoc;
	GLuint shadowTexLoc;
	GLuint numShadowLoc;

	GLuint shadowsSSBOID;
	GLuint shadowMatrixLoc;

	GLuint lightLoc;
	GLuint wLSMLoc;

	float gamma;
	float exposure;
public:
	UIManager();
	~UIManager();

	bool create(int width, int height, std::string title, int fps, std::string postProcessing = "windowFragment");
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
	void setShadowMap(Camera& camera);

	void delay(double timeDelay);

	void display(Camera& camera);

	//parameters
	RenderTexture renderTexture;
	ShadowTexture shadowTexture;
	GLFWwindow* window;
	int width, height;
	double aspectRatio;
	double timeDelay;
	int fps;
	bool mouseLockedState;
	double deltaMouseX, deltaMouseY;
	double r, g, b;
	std::string title;
	std::shared_ptr<GameObject> stage;
	std::shared_ptr<GameObject> HUD;
	std::vector<std::shared_ptr<Light>> lights;
	double deltaTime;
	GLuint windowVBO;
	GLuint windowVAO;
	std::string postProcessingFile;
	GLuint renderTextureID;
	std::shared_ptr <Shader> windowShader;
	std::shared_ptr <Shader> shadowShader;
};


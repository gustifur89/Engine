#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "RenderTexture.h"

class UIManager
{
private:
	double oldTime;
	double oldMouseX, oldMouseY;
	void renderWindow();
	GLuint clearColorLoc;
public:
	UIManager();
	~UIManager();

	bool create(int width, int height, std::string title, int fps, std::string postProcessing = "windowFragment");
	void setUpWindowQuad(std::string postProcessing);

	void initializeWindowShaderUniforms();

	void updateWindowShaderUniforms();

	void glSetUp();
	void setClearColor(int r, int g, int b);
	
	bool isWindowOpen();
	bool isKeyPressed(int key);
	bool isMouseDown(int mouseButton);;
	double getMouseX();
	double getMouseY();
	void lockMouse();
	void unlockMouse();
	bool getMouseLockState();
	void setDeltaMouse();

	void delay(double timeDelay);

	void display(Camera& camera);

	//parameters
	RenderTexture renderTexture;
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
	double deltaTime;
	GLuint windowVBO;
	GLuint windowVAO;
	std::string postProcessingFile;
	GLuint renderTextureID;
	std::shared_ptr <Shader> windowShader;
};


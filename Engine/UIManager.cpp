#include "UIManager.h"



UIManager::UIManager()
{
	window = NULL;
	oldTime = 0.0;
	deltaTime = 0.0;
	mouseLockedState = false;
}

void UIManager::setUpWindowQuad(std::string postProcessing)
{
	// The fullscreen quad's FBO
	glGenVertexArrays(1, &windowVAO);
	glBindVertexArray(windowVAO);

	static const GLfloat windowVertexBufferData[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f,  1.0f, 0.0f,
	};

	glGenBuffers(1, &windowVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertexBufferData), windowVertexBufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					// attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	windowShader = Shader::loadShader("windowVertex" , postProcessing);
	windowShader->getUniformLocation("clearColor");
	renderTextureID = glGetUniformLocation(windowShader->programID, "renderedTexture");
}

bool UIManager::create(int width, int height, std::string title, int fps, std::string postProcessing)
{
	this->width = width;
	this->height = height;
	this->title = title;
	this->aspectRatio = (double)width / height;
	this->fps = fps;
	this->timeDelay = 1.0 / fps;

	glewExperimental = true;
	if (!glfwInit())
	{
		fprintf(stderr, "glfw failed to init");
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	//window; // (In the accompanying source code, this variable is global for simplicity)
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	//fprintf(stdout, "stuff %i", window);

	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

//	glSetUp();

//	glViewport(0, 0, width, height);

	stage = std::shared_ptr<GameObject>(new GameObject());
	HUD = std::shared_ptr<GameObject>(new GameObject());

	setUpWindowQuad(postProcessing);
	initializeWindowShaderUniforms();

	renderTexture = RenderTexture(width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);

	glSetUp();

	glViewport(0, 0, width, height);

	return true;
}

void UIManager::initializeWindowShaderUniforms()
{
	GLint matX = glGetUniformLocation(windowShader->programID, "convolutionMatrixX");
	GLint matY = glGetUniformLocation(windowShader->programID, "convolutionMatrixY");
	
	float sobelX_array[9] =
	{
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};
	float sobelY_array[9] =
	{
		-1, -2, -1,
		 0,  0,  0,
		 1,  2,  1
	};


	glm::mat3 conMatX = glm::mat3(	-1, -2, -1, 
									0,	0,	0, 
									1,	2,	1);

	glm::mat3 conMatY = glm::mat3(	-1, 0, 1, 
									-2, 0, 2, 
									-1, 0, 1);


	windowShader->useShader();
	windowShader->loadMatrix(matX, conMatX);
	windowShader->loadMatrix(matY, conMatY);
}

void UIManager::updateWindowShaderUniforms()
{

}

void UIManager::glSetUp()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
//	glLoadIdentity();
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_STENCIL);
//	glEnable(GL_BLEND);
	//glDisable(GL_BLEND);
}

void UIManager::setClearColor(int r, int g, int b)
{
	this->r = r / 255.0;
	this->g = g / 255.0;
	this->b = b / 255.0;
	glClearColor(this->r, this->g, this->b, 1.0);
	windowShader->loadVector(clearColorLoc, glm::vec3(this->r, this->g, this->b));
}

bool UIManager::isKeyPressed(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool UIManager::isMouseDown(int mouseButton) {
	return glfwGetMouseButton(window, mouseButton) == 1;
}

double UIManager::getMouseX() {
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	return xPos;
}

double UIManager::getMouseY() {
	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	return yPos;
}

void UIManager::lockMouse() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	mouseLockedState = true;
}

void UIManager::unlockMouse() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	mouseLockedState = false;
}

bool UIManager::getMouseLockState() {
	return mouseLockedState;
}

void UIManager::setDeltaMouse() {
	deltaMouseX = getMouseX() - oldMouseX;
	deltaMouseY = getMouseY() - oldMouseY;
	oldMouseX += deltaMouseX;
	oldMouseY += deltaMouseY;
}

bool UIManager::isWindowOpen()
{
	double sTime = glfwGetTime();

	while(glfwGetTime() - sTime < timeDelay)
	{

	}

	return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
}

void UIManager::delay(double timeDelay)
{
	double sTime = glfwGetTime();

	while (glfwGetTime() - sTime < timeDelay)
	{

	}
}

void UIManager::renderWindow()
{
	//bind window
	glBindTexture(GL_TEXTURE_2D, renderTexture.frameBuffer);
	updateWindowShaderUniforms();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, width, height);
	//render the window quads and push the texture
	windowShader->useShader();
	glBindVertexArray(windowVAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	
	//windowShader->load
}

void UIManager::display(Camera& camera)
{
	deltaTime = glfwGetTime() - oldTime;
	oldTime += deltaTime;
	setDeltaMouse();
	if (!window) return;
	//bind renderTexture

	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	stage->render(camera);

	//glClear(GL_DEPTH_BUFFER_BIT);

	//HUD->render(camera);

	//render the window
	renderWindow();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

UIManager::~UIManager()
{
}

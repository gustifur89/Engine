#include "UIManager.h"



UIManager::UIManager()
{
	window = NULL;
	oldTime = 0.0;
	deltaTime = 0.0;
	mouseLockedState = false;
	frameRates = std::vector<double>(maxAvg);
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

	std::string preprocessInstructions;

	if (useShadow)
		preprocessInstructions += "#define USE_SHADOW\n";
	if (useSSAO)
		preprocessInstructions += "#define USE_SSAO\n";
	if (useSobel)
		preprocessInstructions += "#define USE_SOBEL\n";

	windowShader = Shader::loadShaderPreprocess("windowVertex2", postProcessing, preprocessInstructions);
	clearColorLoc = windowShader->getUniformLocation("clearColor");
	gammaLoc = windowShader->getUniformLocation("gamma");
	exposureLoc = windowShader->getUniformLocation("exposure");
	renderTextureID = glGetUniformLocation(windowShader->programID, "renderedTexture");

	colTexLoc = windowShader->getUniformLocation("colTex");
	posTexLoc = windowShader->getUniformLocation("posTex");
	normTexLoc = windowShader->getUniformLocation("normTex");
	shadowTexLoc = windowShader->getUniformLocation("shadowMap");
	ssaoTexLoc = windowShader->getUniformLocation("ssaoTex");
	numShadowLoc = windowShader->getUniformLocation("numShadows");
	wLSMLoc = windowShader->getUniformLocation("lsm");
	gLDirLoc = windowShader->getUniformLocation("globalLightDir");
	gLIntenLoc = windowShader->getUniformLocation("globalLightInten");
	useSSAOLoc = windowShader->getUniformLocation("useSSAO");
	useShadowLoc = windowShader->getUniformLocation("useShadow");
	ambientLoc = windowShader->getUniformLocation("ambient");
	VSMatLoc = windowShader->getUniformLocation("VSMat");
	invVSMatLoc = windowShader->getUniformLocation("invVSMat");
	windowScaleLoc = windowShader->getUniformLocation("windowScale");
	windowShader->useShader();
	glUniform2f(windowScaleLoc, (float) 1.0 / width, (float) 1.0 / height);
	glUniform1i(useSSAOLoc, useSSAO?1:0);
	glUniform1i(useShadowLoc, useShadow?1:0);
	glUniform1i(numShadowLoc, 0);
//std::cout << colTexLoc << "\n";
//std::cout << posTexLoc << "\n";
//std::cout << normTexLoc << "\n";
//std::cout << shadowTexLoc << "\n";
//std::cout << numShadowLoc << "\n";
//std::cout << wLSMLoc << "\n";
	
	if (useComic)
	{
		comicShadingTexture = Texture::loadFromFile("Comic_shade_0.png");


	}


	
	windowShader->useShader();
	glUniform1i(colTexLoc, 0);
	glUniform1i(posTexLoc, 1);
	glUniform1i(normTexLoc, 2);
	if(useShadow)
		glUniform1i(shadowTexLoc, 3);
	if(useSSAO)
		glUniform1i(ssaoTexLoc, 4);
	if (useSobel)
	{
		GLint matX = glGetUniformLocation(windowShader->programID, "conMatX");
		GLint matY = glGetUniformLocation(windowShader->programID, "conMatY");

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

		glm::mat3 conMatX = glm::mat3(-1, -2, -1,
			0, 0, 0,
			1, 2, 1);

		glm::mat3 conMatY = glm::mat3(-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1);

		windowShader->useShader();
		windowShader->loadMatrix(matX, conMatX);
		windowShader->loadMatrix(matY, conMatY);
	}

	if (useSSAO)
	{
		//ssaoShader = Shader::loadShader("SSAO", "SSAO");
		ssaoShader = Shader::loadShader("AO", "AO");
		ssaoShader->useShader();
		ssaoPosTexLoc = ssaoShader->getUniformLocation("posTex");
		ssaoNormTexLoc = ssaoShader->getUniformLocation("normTex");
		noiseTextureLoc = ssaoShader->getUniformLocation("noiseTex");
		samplesLoc = ssaoShader->getUniformLocation("samples");
		projectionLoc = ssaoShader->getUniformLocation("projection");
		noiseScaleLoc = ssaoShader->getUniformLocation("noiseScale");
		viewMatrixLoc = ssaoShader->getUniformLocation("viewMatrix");
		ssaowindowScaleLoc = ssaoShader->getUniformLocation("windowScale");
		glUniform2f(ssaowindowScaleLoc, (float)1.0 / width, (float)1.0 / height);
		glUniform1i(noiseTextureLoc, 0);
		glUniform1i(ssaoPosTexLoc, 1);
		glUniform1i(ssaoNormTexLoc, 2);

		ssaoBlurShader = Shader::loadShader("blur", "blur");
		ssaoBlurShader->useShader();
		ssaoBlurWindowScaleLoc = ssaoBlurShader->getUniformLocation("windowScale");
		ssaoBlurAOTexLoc = ssaoBlurShader->getUniformLocation("aoTex");
		glUniform1i(ssaoBlurAOTexLoc, 5);
		glUniform2f(ssaoBlurWindowScaleLoc, (float)1.0 / width, (float)1.0 / height);

	}

	if (useShadow)
	{
		shadowShader = Shader::loadShader("shadow", "shadow", "shadow");// , "shadow");
		shadowMatrixLoc = shadowShader->getUniformLocation("MV");
		numShadowSSLoc = shadowShader->getUniformLocation("numShadows");
		lightLoc = shadowShader->getUniformLocation("lvm");

		//shadowShader->useShader();
		//GLuint shadowTexLocS = shadowShader->getUniformLocation("shadowMap");
		//glUniform1i(shadowTexLocS, 5);
	}
	
}

bool UIManager::create(int width, int height, std::string title, int fps, unsigned int parameters, std::string postProcessing)
{
	this->width = width;
	this->height = height;
	this->title = title;
	this->aspectRatio = (double)width / height;
	this->fps = fps;
	this->timeDelay = 1.0 / fps;

	useSSAO = (parameters & UI_SSAO) == UI_SSAO;
	useShadow = (parameters & UI_SHADOW) == UI_SHADOW;
	useSobel = (parameters & UI_SOBEL) == UI_SOBEL;
	useComic = (parameters & UI_COMIC) == UI_COMIC;

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
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profilea
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

	if(useSSAO)
		ssaoTexture = SSAOTexture(width, height);

	if (useShadow)
	{
		shadowTexture = ShadowTexture(1024, 32);

		std::vector<GLfloat> lightSSBOData;

		shadowsSSBOID = 0;
		glGenBuffers(1, &shadowsSSBOID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, shadowsSSBOID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 0, &lightSSBOData[0], GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, shadowsSSBOID);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);

	glSetUp();

	glViewport(0, 0, width, height);

	std::cout << glGetString(GL_VERSION) << "\n";

	return true;
}

void UIManager::initializeWindowShaderUniforms()
{
	/*
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


	glm::mat3 conMatX = glm::mat3(-1, -2, -1,
		0, 0, 0,
		1, 2, 1);

	glm::mat3 conMatY = glm::mat3(-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1);


	windowShader->useShader();
	windowShader->loadMatrix(matX, conMatX);
	windowShader->loadMatrix(matY, conMatY);
	*/
}

void UIManager::updateWindowShaderUniforms()
{

}

void UIManager::glSetUp()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glEnable(GL_NORMALIZE);
	//	glLoadIdentity();
		//glEnable(GL_TEXTURE_2D);
		//glEnable(GL_STENCIL);
	//	glEnable(GL_BLEND);
		//glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
}

void UIManager::setClearColor(int r, int g, int b)
{
	this->r = r / 255.0;
	this->g = g / 255.0;
	this->b = b / 255.0;
	glClearColor(this->r, this->g, this->b, 1.0);
	windowShader->loadVector(clearColorLoc, glm::vec3(this->r, this->g, this->b));
}

void UIManager::addLight(std::shared_ptr<Light> light)
{
	lights.push_back(light);
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

	while (glfwGetTime() - sTime < timeDelay)
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
	//glBindTexture(GL_TEXTURE_2D, renderTexture.frameBuffer);

	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture.colTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, renderTexture.posTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, renderTexture.normTex);

	glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTexture.texID);
	glBindTexture(GL_TEXTURE_2D, shadowTexture.depthMap);

	*/
	glBindTextureUnit(0, renderTexture.colTex);
	glBindTextureUnit(1, renderTexture.posTex);
	glBindTextureUnit(2, renderTexture.normTex);
//	glBindTextureUnit(3, shadowTexture.depthMap);
	
	if (useShadow)
	{
		glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTexture.texID);

		glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTexture.depthMap);// GL_TEXTURE_2D_ARRAY   GL_TEXTURE_2D
	}

	if(useSSAO)
		glBindTextureUnit(4, ssaoTexture.aoTexture);

	updateWindowShaderUniforms();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, width, height);
	//render the window quads and push the texture
	windowShader->loadVector(clearColorLoc, glm::vec3(this->r, this->g, this->b));
	windowShader->useShader();
	windowShader->loadVector(gLDirLoc, Light::globalLightDirection);
	windowShader->loadFloat(gLIntenLoc, Light::globalLightIntensity);
	windowShader->loadFloat(ambientLoc, Light::ambient);
	glBindVertexArray(windowVAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	//windowShader->load
}

void UIManager::renderSSAO(Camera & camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoTexture.frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	ssaoShader->useShader();
	glBindTextureUnit(0, ssaoTexture.noiseTexture);
	glBindTextureUnit(1, renderTexture.posTex);
	glBindTextureUnit(2, renderTexture.normTex);

	glUniform3fv(samplesLoc, ssaoTexture.kernelSamples.size(), &ssaoTexture.kernelSamples[0]);
	ssaoShader->loadMatrix(projectionLoc, camera.getProjection());
	//ssaoShader->loadMatrix(projectionLoc, glm::inverse(camera.getProjection()));
	ssaoShader->loadMatrix(viewMatrixLoc, camera.getTransform());
	glUniform2f(noiseScaleLoc, width / 4.0, height / 4.0);

	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(windowVAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//BLur

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoTexture.frameBuffer);
	ssaoBlurShader->useShader();
	glBindTextureUnit(5, ssaoTexture.aoTexture);
	glBindVertexArray(windowVAO);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void UIManager::setHDR(float gamma, float exposure)
{
	windowShader->useShader();
	windowShader->loadFloat(gammaLoc, gamma);
	windowShader->loadFloat(exposureLoc, exposure);
	this->gamma = gamma;
	this->exposure = exposure;
}

std::vector<std::shared_ptr<Light>> UIManager::cullLights(Camera& camera)
{
	std::vector<std::shared_ptr<Light>> lightsOut;

	for (int i = 0; i < lights.size(); i++)
	{
		if (camera.isLightInView(lights[0]))
		{
			lightsOut.push_back(lights[i]);
		}
	}


	return lightsOut;
}

int UIManager::setShadowMap(Camera& camera, std::vector<std::shared_ptr<Light>> lights_)
{
	//do things with lights
	//TODO: cull the lights if they are outside the camera's viewing frustum

	/*
	struct Light {
		vec3 pos;
		float radius;
		float intensity;
		mat4 projectionViewMatrix;
		float padding;
	};
	*/

	std::vector<GLfloat> lightSSBO;

	int numShadows = 0;

	for (int i = 0; i < lights_.size(); i++)
	{
		for (int j = 0; j < lights_[i]->projectionMatrix.size(); j++)
		{
			numShadows++;

			lightSSBO.push_back((float)lights_[i]->position.x);
			lightSSBO.push_back((float)lights_[i]->position.y);
			lightSSBO.push_back((float)lights_[i]->position.z);
			lightSSBO.push_back(0.0f);
			glm::mat4 mat = lights_[i]->getProjection(j) * glm::inverse(camera.getTransform());

			//GLfloat * stuff = gl
			//float * projMat = &mat[0][0];// glm::value_ptr(mat);
			//
			//for (int k = 0; k < 16; k++)
			//{
			//	lightSSBO.push_back((float)projMat[k]);
			//}

			for (int r = 0; r < 4; r ++)
			{
				for (int c = 0; c < 4; c ++)
				{
					lightSSBO.push_back(mat[r][c]);
				}
				
			}
		//	&matrix[0][0]
			lightSSBO.push_back(0.0f);
			lightSSBO.push_back((float)lights_[i]->radius);
			lightSSBO.push_back((float)lights_[i]->intensity);

			lightSSBO.push_back(0.0f);
		//	lightSSBO.push_back((float)lights[i]->color.x / 255.0f);
		//	lightSSBO.push_back((float)lights[i]->color.y / 255.0f);
		//	lightSSBO.push_back((float)lights[i]->color.z / 255.0f);
		//	lightSSBO.push_back(0.0f);
		//	lightSSBO.push_back(0.0f);
		}
	}

	//std::cout << numShadows << "\n";

	//send the data over the ssbo
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, shadowsSSBOID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, lightSSBO.size() * sizeof(GLfloat), &lightSSBO[0], GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, shadowsSSBOID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);


	//shadowShader->loadMatrix(lightLoc, lights[0]->getProjection(0));



	glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTexture.texID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowTexture.depthMap);// GL_TEXTURE_2D_ARRAY   GL_TEXTURE_2D
	return numShadows;
}

void UIManager::display(Camera& camera, std::shared_ptr<GameObject> renderObject)
{
	deltaTime = glfwGetTime() - oldTime;
	oldTime += deltaTime;
	setDeltaMouse();
	if (!window) return;
	//bind renderTexture
	if (camera.exposure != exposure || camera.gamma != gamma)
		setHDR(camera.gamma, camera.exposure);

	//	glClear(GL_DEPTH_BUFFER_BIT);
	//	setShadowMap(camera);
	//	glViewport(0, 0, shadowTexture.size, shadowTexture.size);
	//	stage->renderShadow(windowShader, shadowMatrixLoc);

	//	glBindFramebuffer(GL_FRAMEBUFFER, shadowTexture.frameBuffer);
	//	glViewport(0, 0, shadowTexture.size, shadowTexture.size);
	//	glClear(GL_DEPTH_BUFFER_BIT);
	//	setShadowMap(camera);
	//	stage->renderShadow(windowShader, shadowMatrixLoc);
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if(useShadow)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, shadowTexture.depthMapFBO);
		glViewport(0, 0, 1024, 1024);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader->useShader();
		std::vector<std::shared_ptr<Light>> lights_ = cullLights(camera);
		numShadows = setShadowMap(camera, lights_);
		glUniform1i(numShadowSSLoc, numShadows);
		glCullFace(GL_FRONT);
		stage->renderShadow(lights_, windowShader, shadowMatrixLoc, glm::mat4(1.0));
		glCullFace(GL_BACK);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set up shadows
	renderObject->render(camera, glm::mat4(1.0));

	//glClear(GL_DEPTH_BUFFER_BIT);

	//HUD->render(camera);

	//render the window

	if(useSSAO)	
		renderSSAO(camera);

	windowShader->useShader();
	if(useShadow)
		glUniform1i(numShadowLoc, numShadows);

	windowShader->loadMatrix(VSMatLoc, camera.getTransform());
	windowShader->loadMatrix(invVSMatLoc, glm::inverse(camera.getTransform()));

	renderWindow();

	glfwSwapBuffers(window);
	glfwPollEvents();

//	std::cout << (1.0 / deltaTime) << "\n";
#ifdef PRINT_FPS
	curFrame++;
	if (curFrame >= maxAvg)
	{
		double avg = 0.0;
		for (int i = 0; i < maxAvg; i++)
		{
			avg += frameRates[i];
		}
		std::cout << (avg / maxAvg) << "\n";
		curFrame = 0;
	}
	frameRates[curFrame] = (1.0 / deltaTime);
#endif
}

UIManager::~UIManager()
{

}

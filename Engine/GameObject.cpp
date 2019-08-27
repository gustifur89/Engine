#include "GameObject.h"

// ============================== GameObject ===========================

GameObject::GameObject()
{
	visible = true;
	collidable = true;
}

void GameObject::renderFunc(Camera& camera)
{
	//no nothing
}

void GameObject::render(Camera& camera)
{
	if (!this || !visible) return;
	this->renderFunc(camera);
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->render(camera);
	}
}

void GameObject::addChild(std::shared_ptr<GameObject> gameObject)
{
	children.push_back(gameObject);
}

GameObject::~GameObject()
{
	
}

// ============================== GameObjectColor ===========================

GameObjectColor::GameObjectColor()
{
	colorMatrix = glm::mat4(1.0);
}

void GameObjectColor::setFillColor(int r, int g, int b)
{
	colorMatrix = glm::mat4(0.0);
	colorMatrix[3][0] = r / 255.f;
	colorMatrix[3][1] = g / 255.f;
	colorMatrix[3][2] = b / 255.f;
	colorMatrix[3][3] = 1.0f;
}

void GameObjectColor::renderFunc(Camera& camera)
{
	glm::mat4 MVPmatrix = camera.getProjection() * camera.getTransform() * transform.getTransform();
	glm::mat4 NMmatrix = transform.getTransform();  // glm::transpose(glm::inverse(transform.getTransform()));
	/*
	//shader->put matrixes
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) camera.aspectRatio, 0.1f, 100.0f);

	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 mvp = Projection * View * transform.getTransform();

	shader->useShader();

	GLuint MatrixID = glGetUniformLocation(shader->programID, "MVP");
	*/
	if (shader && mesh)
	{
		shader->useShader();
		shader->setLightInternal(shader->light);
		shader->setMatrixes(MVPmatrix, NMmatrix, colorMatrix);
		mesh->render();
	}
}

GameObjectColor::~GameObjectColor()
{

}

// ============================== GameObjectTexture ===========================

GameObjectTexture::GameObjectTexture()
{

}

void GameObjectTexture::renderFunc(Camera& camera)
{
	glm::mat4 MVPmatrix = camera.getProjection() * camera.getTransform() * transform.getTransform();
	glm::mat4 NMmatrix = transform.getTransform(); 
	
	if (shader && mesh && texture)
	{
		shader->useShader();
		shader->setLightInternal(shader->light);
		shader->setTexture(texture);
		shader->setMatrixes(MVPmatrix, NMmatrix);
		mesh->render();
	}
}

GameObjectTexture::~GameObjectTexture()
{

}

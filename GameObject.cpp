#include "GameObject.h"

// ============================== GameObject ===========================

GameObject::GameObject()
{
	visible = true;
	collidable = true;
	persistentVisible = false;
}

void GameObject::renderFunc(Camera& camera, glm::mat4 parentTransform)
{
	//no nothing
}

void GameObject::renderShadow(std::vector<std::shared_ptr<Light>> lights, std::shared_ptr<Shader> ShadowShader, int location, glm::mat4 parentTransform)
{
	if (!this || !visible) return;
	///*
	if (mesh)
	{
		if (Light::isBoxInListView(mesh->bounds, this->transform.getTransform(), lights))
		{
			this->renderShadowFunc(ShadowShader, location, parentTransform);
		}
	}
	//*/
	//this->renderShadowFunc(ShadowShader, location);
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->renderShadow(lights, ShadowShader, location, parentTransform * this->transform.getTransform());
	}
}

void GameObject::renderShadowFunc(std::shared_ptr<Shader> ShadowShader, int location, glm::mat4 parentTransform)
{
	glm::mat4 MVmatrix = parentTransform * transform.getTransform();
	//std::cout << "yeet?\n";
	if (ShadowShader && mesh)
	{
		//	std::cout << "meat?\n";
		//	std::cout << location << "\n";
		ShadowShader->loadMatrix(location, MVmatrix);
		mesh->render();
	}
}

Bounds GameObject::getWorldSpaceBounds()
{
	if (!mesh)
	{
		//then no mesh. just retrun the postion
		return Bounds(transform.position, transform.position);
	}
	else
	{

	}

	return Bounds();
}

void GameObject::render(Camera& camera, glm::mat4 parentTransform)
{
	if (!this || !visible) return;
//	this->renderFunc(camera);
	///*
	if (mesh)
	{
		if (camera.isBoxInView(mesh->bounds, parentTransform * this->transform.getTransform()))
		{
			this->renderFunc(camera, parentTransform);
		}
		else if (this->persistentVisible)
		{
		//	std::cout << " Draw\n";
			this->renderFunc(camera, parentTransform);
		}
	}
	else if (this->persistentVisible)
	{
		this->renderFunc(camera, parentTransform);
	}
	//*/
	//this->renderFunc(camera);

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->render(camera, parentTransform * this->transform.getTransform());
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

void GameObjectColor::renderFunc(Camera& camera, glm::mat4 parentTransform)
{
	glm::mat4 MVMatrix = camera.getTransform() * parentTransform * transform.getTransform();
	glm::mat4 MVPmatrix = camera.getProjection() * MVMatrix;
	glm::mat4 NMmatrix = glm::transpose(glm::inverse(MVMatrix));
	//IS NM is now frm mesh to view space!!
																			
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
		shader->setMatrixes(MVPmatrix, MVMatrix, NMmatrix, colorMatrix);
		mesh->render();
	}
}

/*
void GameObjectColor::renderShadowFunc(std::shared_ptr<Shader> ShadowShader, int location)
{
	glm::mat4 MVmatrix = transform.getTransform();
	//std::cout << "yeet?\n";
	if (ShadowShader && mesh)
	{
	//	std::cout << "meat?\n";
	//	std::cout << location << "\n";
		ShadowShader->loadMatrix(location, MVmatrix);
		mesh->render();
	}
}
*/

GameObjectColor::~GameObjectColor()
{

}

// ============================== GameObjectTexture ===========================

GameObjectTexture::GameObjectTexture()
{

}

void GameObjectTexture::renderFunc(Camera& camera, glm::mat4 parentTransform)
{
	glm::mat4 MVMatrix = camera.getTransform() * parentTransform * transform.getTransform();
	glm::mat4 MVPmatrix = camera.getProjection() * MVMatrix;
	glm::mat4 NMmatrix = glm::transpose(glm::inverse(MVMatrix));

	if (shader && mesh && texture)
	{
		shader->useShader();
		shader->setLightInternal(shader->light);
		shader->setTexture(texture);
		shader->setMatrixes(MVPmatrix, MVMatrix, NMmatrix);
		mesh->render();
	}
}

/*
void GameObjectTexture::renderShadowFunc(std::shared_ptr<Shader> shader, int location)
{
	glm::mat4 MVmatrix = transform.getTransform();
	if (shader && mesh)
	{
		shader->loadMatrix(location, MVmatrix);
		mesh->render();
	}
}
*/

GameObjectTexture::~GameObjectTexture()
{

}

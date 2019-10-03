#include "Light.h"

//========================= Light ====================

glm::vec3 Light::globalLightDirection = glm::vec3(0, 0, 0);
double Light::globalLightIntensity = 0.0;
double Light::ambient = 0.0;

Light::Light()
{
	numSubLights = 0;
}

Light::~Light()
{

}

glm::mat4 Light::getViewTransform(int index)
{
	glm::vec3 direction = dir[index];
	glm::vec3 directionCompliment = dirComp[index];

	glm::vec3 lookAt = position + direction;
	glm::mat4 matrix = glm::lookAt(position, lookAt, directionCompliment);
	return matrix;
}

glm::mat4 Light::getProjection(int index)
{
	glm::mat4 projection = projectionMatrix[index];
//glm::vec3 direction = dir[index];
//glm::vec3 directionCompliment = dirComp[index];
//
//glm::vec3 lookAt = position + direction;
//glm::mat4 matrix = glm::lookAt(position, lookAt, directionCompliment);

	/*
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	return lightProjection * lightView;
	//*/
	return projection * getViewTransform(index);
}

void Light::addIndividualSpotLight(glm::vec3 pos, glm::vec3 direction, double radius, double intensity, glm::vec3 color)
{
	glm::mat4 projection = glm::perspective(
		glm::radians(90.0f),	// The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		1.0f,						// Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,						// Near clipping plane. Keep as big as possible, or you'll get precision issues.
		(float) radius				// Far clipping plane. Keep as little as possible.
	);

	viewFrustum = Frustum(projection);
	//glm::vec3 lookAt = pos + direction;
	//glm::mat4 matrix = glm::lookAt( pos, lookAt, glm::vec3(0, 1, 0) );

	//glm::mat4 MVPmatrix = camera.getProjection() * camera.getTransform()

	//projectionMatrix.push_back(projection * matrix);
	this->radius = radius;
	this->intensity = intensity;
	this->color = color;
	projectionMatrix.push_back(projection);
	glm::vec3 directionCompliment(0, 1, 0);
	numSubLights++;
	if (direction == glm::vec3(0, 1, 0) || direction == glm::vec3(0, -1, 0))
	{
		directionCompliment = glm::vec3(1, 0, 0);
	}
	dir.push_back(glm::normalize(direction));

	dirComp.push_back(glm::normalize(directionCompliment));
}

std::shared_ptr<Light> Light::createPointLight(glm::vec3 pos, double radius, double intensity, glm::vec3 color)
{
	std::shared_ptr<Light> light = std::shared_ptr<Light>(new Light());
	
	light->addIndividualSpotLight(pos, glm::vec3(1, 0, 0), radius, intensity, color);
	light->addIndividualSpotLight(pos, glm::vec3(-1, 0, 0), radius, intensity, color);
	light->addIndividualSpotLight(pos, glm::vec3(0, 1, 0), radius, intensity, color);
	light->addIndividualSpotLight(pos, glm::vec3(0, -1, 0), radius, intensity, color);
	light->addIndividualSpotLight(pos, glm::vec3(0, 0, 1), radius, intensity, color);
	light->addIndividualSpotLight(pos, glm::vec3(0, 0, -1), radius, intensity, color);

	light->position = pos;

	return light;
}

std::shared_ptr<Light> Light::createSpotLight(glm::vec3 pos, glm::vec3 rotation, double radius, double intensity, glm::vec3 color)
{
	std::shared_ptr<Light> light = std::shared_ptr<Light>(new Light());

	glm::vec4 lookDirection(0, 0, 1, 0);	//Looking direction in viewSpace
	//rotate by the direction
	glm::mat4 rotationMat = glm::toMat4(glm::quat(glm::vec3(rotation.x * TO_RAD, rotation.y * TO_RAD, rotation.z * TO_RAD)));
	//roate the look direction by the rotation
	lookDirection = rotationMat * lookDirection;

	light->addIndividualSpotLight(pos, lookDirection, radius, intensity, color);

	light->position = pos;

	return light;
}

bool Light::isBoxInView(Bounds bounds)
{
	//Box in world space
		//put vectors into view space.
	Bounds bounds_;
	for (int i = 0; i < numSubLights; i++)
	{
		//bounds_ = bounds.applyMatrix(getViewTransform(i));
		if (viewFrustum.viewSpaceBoxInFrustum(getViewTransform(i), bounds_))
			return true;
	}
	return false;
}

bool Light::isBoxInListView(Bounds bounds, glm::mat4 modelMatrix, std::vector<std::shared_ptr<Light>> lights)
{
	bounds = bounds.applyMatrix(modelMatrix);
	//problem with AABB not transforming into rotated space
	for (int i = 0; i < lights.size(); i++)
	{
		if (lights[i]->isBoxInView(bounds))
			return true;
	}
	return false;
}

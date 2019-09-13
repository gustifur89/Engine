#pragma once
#include "Headers.h"
#include "Transform.h"
#include "Light.h"

class Camera : public Transform
{
private:



public:
	Camera(float fov, float aspectRatio, float minZ, float maxZ);
	~Camera();

	glm::mat4 getTransform();
	glm::mat4 getProjection();
	

	bool isLightInView(std::shared_ptr<Light> light);

	bool inView(glm::vec3 p);
	//parameters
	glm::mat4 projectionMatrix;
	GLfloat fov;
	GLfloat aspectRatio;
	GLfloat minZ;
	GLfloat maxZ;
	GLfloat gamma;
	GLfloat exposure;
};


#pragma once
#include "Headers.h"
#include "Transform.h"

class Camera : public Transform
{
public:
	Camera(float fov, float aspectRatio, float minZ, float maxZ);
	~Camera();

	glm::mat4 getTransform();
	glm::mat4 getProjection();
	
	bool inView(glm::vec3 p);
	//parameters
	glm::mat4 projectionMatrix;
	GLfloat fov;
	GLfloat aspectRatio;
	GLfloat minZ;
	GLfloat maxZ;
};


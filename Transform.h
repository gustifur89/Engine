#pragma once
#include "Headers.h"
class Transform
{
public:
	Transform();
	~Transform();

	glm::mat4 getTransform();
	glm::mat4 getRotation();
	glm::mat4 getScale();
	glm::mat4 getTranslate();

	glm::vec3 getTransformedX();
	glm::vec3 getTransformedY();
	glm::vec3 getTransformedZ();

	static glm::vec3 getTransformedX(glm::vec3 angles);
	static glm::vec3 getTransformedY(glm::vec3 angles);
	static glm::vec3 getTransformedZ(glm::vec3 angles);

	glm::vec3 getPosition();
	//parameters
	double x, y, z;
	double angleX, angleY, angleZ;
	double scaleX, scaleY, scaleZ;
};


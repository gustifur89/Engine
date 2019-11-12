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

	inline Transform operator+(const Transform& other)
	{
		Transform out;
		out.position = this->position + other.position;
		out.rotation = this->rotation + other.rotation;
		out.scale = this->scale + other.scale;
		return out;
	}

//	glm::vec3 getPosition();
	//parameters
//	double x, y, z;
//	double angleX, angleY, angleZ;
//	double scaleX, scaleY, scaleZ;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};


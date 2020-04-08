#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0);	
	setRotation(0, 0, 0);
	scale = glm::vec3(1.0);
}

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	this->position = pos;
	setRotation(rot);
	this->scale = scale;
}

glm::mat4 Transform::getRotationMatrix(glm::vec3 angles)
{
	return glm::toMat4(glm::quat(glm::radians(angles)));
}

glm::mat4 Transform::getRotationMatrix(glm::quat rotQuat)
{
	return glm::toMat4(rotQuat);
}

glm::vec3 Transform::getTransformedZ()
{
	glm::mat4 rotationMatrix = getRotationMatrix(rotation);
	glm::vec4 zAxis(0, 0, 1, 0);
	
	return rotationMatrix * zAxis;
}

glm::vec3 Transform::getTransformedZ(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = getRotationMatrix(angles);
	glm::vec4 zAxis(0, 0, 1, 0);

	return rotationMatrix * zAxis;
}

glm::vec3 Transform::getTransformedX()
{
	glm::mat4 rotationMatrix = getRotationMatrix(rotation);
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotationMatrix * xAxis;
}

glm::vec3 Transform::getTransformedX(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = getRotationMatrix(angles);
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotationMatrix * xAxis;
}

glm::vec3 Transform::getTransformedY()
{
	glm::mat4 rotationMatrix = getRotationMatrix(rotation);
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotationMatrix * yAxis;
}

glm::vec3 Transform::getTransformedY(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = getRotationMatrix(angles);
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotationMatrix * yAxis;
}

glm::mat4 Transform::getTransform()
{
	//glm::quat rotationQuat;
	//rotationQuat = glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD));

	glm::mat4 translation = glm::translate(position);;// glm::translate(glm::mat4(1.0), position);

	glm::mat4 rotationMatrix = getRotationMatrix(rotation);

	glm::mat4 scaleMatrix = glm::scale(scale);

	return translation * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::getRotation()
{
	return getRotationMatrix(rotation);
}

glm::mat4 Transform::getScale()
{
	return glm::scale(scale);
}

glm::mat4 Transform::getTranslate()
{
	return glm::translate(glm::mat4(1.0), position);
}

void Transform::setRotation(double x, double y, double z)
{
	setRotation(glm::vec3(x, y, z));
}

void Transform::setRotation(glm::vec3 eulerAngles)
{
	rotation = glm::quat(glm::radians(eulerAngles));
}

void Transform::setRotation(Transform transform)
{
	this->rotation = transform.rotation;
}

glm::vec3 Transform::getEulerAngles()
{
	return glm::eulerAngles(rotation);
}

void Transform::rotate(Transform transform)
{
	rotate(transform.rotation);
}

void Transform::rotate(glm::vec3 eulerAngles)
{
	rotate(glm::quat(glm::radians(eulerAngles)));
}

void Transform::rotate(glm::quat rotQuat)
{
	//rotation = rotQuat * rotation * glm::inverse(rotQuat);
	this->rotation = rotation * rotQuat;// this->rotation* rotQuat;
}

Transform::~Transform()
{

}

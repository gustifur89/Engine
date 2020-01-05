#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0);
	rotation = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	rotationMatrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

glm::mat4 Transform::getRotation(glm::vec3 angles)
{
//	glm::mat4 temp = glm::toMat4(glm::quat((float)TO_RAD * angles));
//	glm::quat orient = glm::gtx::quaternion::angleAxis(float(0), 0.0f, 0.0f, 1.0f);
	//glm::mat4 transform = glm::eulerAngleZYX((float)TO_RAD * angles.z, (float)TO_RAD * angles.y, (float)TO_RAD * angles.x);
//	glm::quat rot = glm::angleAxis(glm::radians(angles));// , glm::vec3(x, y, z));
	//glm::mat4 rotMatrix = glm::mat4_cast(rot);

	return glm::toMat4(glm::quat(glm::radians(angles)));// transform;// glm::toMat4(glm::quat((float)TO_RAD * angles));
}

glm::vec3 Transform::getTransformedZ()
{
	glm::mat4 rotationMatrix = getRotation(rotation);
	glm::vec4 zAxis(0, 0, 1, 0);
	
	return rotationMatrix * zAxis;
}

glm::vec3 Transform::getTransformedZ(glm::mat4 rotationMatrix)
{
	glm::vec4 zAxis(0, 0, 1, 0);

	return rotationMatrix * zAxis;
}

glm::vec3 Transform::getTransformedZ(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = getRotation(angles);
	glm::vec4 zAxis(0, 0, 1, 0);

	return rotationMatrix * zAxis;
}

glm::vec3 Transform::getTransformedX()
{
	glm::mat4 rotationMatrix = getRotation(rotation);
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotationMatrix * xAxis;
}

glm::vec3 Transform::getTransformedX(glm::mat4 rotationMatrix)
{
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotationMatrix * xAxis;
}

glm::vec3 Transform::getTransformedX(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = getRotation(angles);
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotationMatrix * xAxis;
}

glm::vec3 Transform::getTransformedY()
{
	glm::mat4 rotationMatrix = getRotation(rotation);
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotationMatrix * yAxis;
}

glm::vec3 Transform::getTransformedY(glm::mat4 rotationMatrix)
{
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotationMatrix * yAxis;
}

glm::vec3 Transform::getTransformedY(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = getRotation(angles);
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotationMatrix * yAxis;
}

/*
glm::vec3 Transform::getPosition()
{
	return glm::vec3(x, y, z);
}
*/

glm::mat4 Transform::getTransform()
{
	//glm::quat rotationQuat;
	//rotationQuat = glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD));

	glm::mat4 translation = glm::translate(glm::mat4(1.0), position);

	glm::mat4 rotationMatrix = getRotation(rotation);

	glm::mat4 scaleMatrix = glm::scale(scale);

	return translation * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::getRotation()
{
	return rotationMatrix;
}

glm::mat4 Transform::getScale()
{
	return glm::scale(scale);
}

glm::mat4 Transform::getTranslate()
{
	return glm::translate(glm::mat4(1.0), position);
}

Transform::~Transform()
{

}

void Transform::setRotation(glm::vec3 rotation)
{
	this->rotationMatrix = glm::toMat4(glm::quat(glm::radians(rotation)));
}

void Transform::setRotation(float x, float y, float z)
{
	this->rotationMatrix = glm::toMat4(glm::quat(glm::radians(glm::vec3(x, y, z))));
}

void Transform::rotate(glm::mat4 rotationMatrix)
{
	this->rotationMatrix = this->rotationMatrix * rotationMatrix;
}

void Transform::rotate(float x, float y, float z)
{
	this->rotate(glm::toMat4(glm::quat(glm::radians(glm::vec3(x,y,z)))));
}

void Transform::rotate(glm::vec3 rotation)
{
	this->rotate(glm::toMat4(glm::quat(glm::radians(rotation))));
}


#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0);
	rotation = glm::vec3(0.0);
	scale = glm::vec3(1.0);
}

glm::vec3 Transform::getTransformedZ()
{
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat((float)TO_RAD * rotation));
	glm::vec4 zAxis(0, 0, 1, 0);
	
	return rotationMatrix * zAxis;
}

glm::vec3 Transform::getTransformedZ(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat((float) TO_RAD * angles));
	glm::vec4 zAxis(0, 0, 1, 0);

	return rotationMatrix * zAxis;
}

glm::vec3 Transform::getTransformedX()
{
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat((float)TO_RAD * rotation));
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotationMatrix * xAxis;
}

glm::vec3 Transform::getTransformedX(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat((float)TO_RAD * angles));
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotationMatrix * xAxis;
}

glm::vec3 Transform::getTransformedY()
{
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat((float)TO_RAD * rotation));
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotationMatrix * yAxis;
}

glm::vec3 Transform::getTransformedY(glm::vec3 angles)
{
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat((float)TO_RAD * angles));
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

	glm::mat4 rotationMatrix = glm::toMat4(glm::quat((float)TO_RAD * rotation));

	glm::mat4 scaleMatrix = glm::scale(scale);

	return translation * rotationMatrix * scaleMatrix;
}

glm::mat4 Transform::getRotation()
{
	return  glm::toMat4(glm::quat((float)TO_RAD * rotation));
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

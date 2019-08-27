#include "Transform.h"

Transform::Transform()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
	angleX = 0.0;
	angleY = 0.0;
	angleZ = 0.0;
	scaleX = 1.0;
	scaleY = 1.0;
	scaleZ = 1.0;
}

glm::vec3 Transform::getTransformedZ()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD)));
	glm::vec4 zAxis(0, 0, 1, 0);
	
	return rotation * zAxis;
}

glm::vec3 Transform::getTransformedZ(glm::vec3 angles)
{
	glm::mat4 rotation = glm::toMat4(glm::quat((float) TO_RAD * angles));
	glm::vec4 zAxis(0, 0, 1, 0);

	return rotation * zAxis;
}

glm::vec3 Transform::getTransformedX()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD)));
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotation * xAxis;
}

glm::vec3 Transform::getTransformedX(glm::vec3 angles)
{
	glm::mat4 rotation = glm::toMat4(glm::quat((float)TO_RAD * angles));
	glm::vec4 xAxis(1, 0, 0, 0);

	return rotation * xAxis;
}

glm::vec3 Transform::getTransformedY()
{
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD)));
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotation * yAxis;
}

glm::vec3 Transform::getTransformedY(glm::vec3 angles)
{
	glm::mat4 rotation = glm::toMat4(glm::quat((float)TO_RAD * angles));
	glm::vec4 yAxis(0, 1, 0, 0);

	return rotation * yAxis;
}

glm::vec3 Transform::getPosition()
{
	return glm::vec3(x, y, z);
}

glm::mat4 Transform::getTransform()
{
	//glm::quat rotationQuat;
	//rotationQuat = glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD));

	glm::mat4 translation = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));

	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD)));

	glm::mat4 scale = glm::scale(glm::vec3(scaleX, scaleY, scaleZ));

	return translation * rotation * scale;
}

glm::mat4 Transform::getRotation()
{
	return glm::toMat4(glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD)));
}

glm::mat4 Transform::getScale()
{
	return glm::scale(glm::vec3(scaleX, scaleY, scaleZ));
}

glm::mat4 Transform::getTranslate()
{
	return glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
}

Transform::~Transform()
{

}

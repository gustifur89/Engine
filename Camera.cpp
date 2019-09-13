#include "Camera.h"



Camera::Camera(float fov, float aspectRatio, float minZ, float maxZ)
{
	projectionMatrix = glm::perspective(
		glm::radians(fov),	// The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		aspectRatio,		// Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		minZ,				// Near clipping plane. Keep as big as possible, or you'll get precision issues.
		maxZ				// Far clipping plane. Keep as little as possible.
	);

	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->minZ = minZ;
	this->maxZ = maxZ;
	this->gamma = 2.2;
	this->exposure = 1.0;
}

Camera::~Camera()
{

}

glm::mat4 Camera::getProjection()
{
	return projectionMatrix;
}

glm::mat4 Camera::getTransform()
{
	glm::vec4 lookDirection(0,0,1, 0);	//Looking direction in viewSpace
	//rotate by the direction
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(angleX * TO_RAD, angleY * TO_RAD, angleZ * TO_RAD)));
	//roate the look direction by the rotation
	lookDirection = rotation * lookDirection;
	glm::vec3 position(x, y, z);
	glm::vec3 lookAt = position + glm::vec3(lookDirection);

	return glm::lookAt(
		position,	// the position of your camera, in world space
		lookAt,		// where you want to look at, in world space
		glm::vec3(0,1,0)    // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	);
}

bool Camera::isLightInView(std::shared_ptr<Light> light)
{
	//checks the collision between the viewing frustum and the light's sphere
	//TODO: add
	return true;
}

bool Camera::inView(glm::vec3 p)
{
	glm::mat4 VPmatrix = getProjection() * getTransform();
	glm::vec4 t_point = VPmatrix * glm::vec4(p, 1.0);
	return abs(t_point.x) < t_point.w &&
		abs(t_point.y) < t_point.w &&
		0.0 < t_point.z &&
		t_point.z < t_point.w;
}

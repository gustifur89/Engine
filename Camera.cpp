#include "Camera.h"



Camera::Camera(float fov, float aspectRatio, float minZ, float maxZ)
{
	projectionMatrix = glm::perspective(
		glm::radians(fov),	// The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		aspectRatio,		// Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		minZ,				// Near clipping plane. Keep as big as possible, or you'll get precision issues.
		maxZ				// Far clipping plane. Keep as little as possible.
	);

	viewFrustum = Frustum(projectionMatrix);
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
	glm::mat4 rotationMatrix = this->getRotation();// glm::toMat4(glm::quat((float)TO_RAD * rotation));
	//roate the look direction by the rotation
	lookDirection = rotationMatrix * lookDirection;
	glm::vec3 lookAt = position + glm::vec3(lookDirection);

	return glm::lookAt(
		position,	// the position of your camera, in world space
		lookAt,		// where you want to look at, in world space
		glm::vec3(0,1,0)    // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
	);
}

bool Camera::isSphereInView(glm::vec3 position, double radius, glm::mat4 modelMatrix)
{
	glm::vec4 pos = glm::vec4(position, 1);
	pos = getTransform() * modelMatrix * pos;
	return viewFrustum.viewSpaceSphereInFrustum(glm::vec3(pos), radius);
}

bool Camera::isBoxInView(Bounds bounds, glm::mat4 modelMatrix)
{
	//camera.getProjection() * camera.getTransform()
	//Box in world space
		//put vectors into view space.
//	bounds = bounds.applyMatrix(getProjection() * getTransform() * modelMatrix);	
//	if (bounds.low.x <= 1.0 && bounds.low.y <= 1.0 && bounds.low.z <= 1.0 && bounds.high.x >= -1.0 && bounds.high.y >= -1.0 && bounds.high.z >= -1.0)
//	{
//		return true;
//	}
//	return false;
			
	//bounds = bounds.applyMatrix(getTransform() * modelMatrix);
	return viewFrustum.viewSpaceBoxInFrustum(getTransform() * modelMatrix, bounds);
}

bool Camera::isLightInView(std::shared_ptr<Light> light)
{
	//checks the collision between the viewing frustum and the light's sphere
	//TODO: add
//	glm::vec3 dP = light->position - this->position;
//	if(length(dP) <= this->maxZ)
//		return true;

	return isSphereInView(light->position, light->radius, glm::mat4(1));

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

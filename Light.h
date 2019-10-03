#pragma once
#include "Headers.h"
#include "Transform.h"
#include "Geometry.h"

class Light : public Transform
{
private: 
	void addIndividualSpotLight(glm::vec3 pos, glm::vec3 direction, double radius, double intensity, glm::vec3 color);

public:
	//methods
	Light();
	~Light();

	glm::mat4 getViewTransform(int index);
	glm::mat4 getProjection(int index);

	static std::shared_ptr<Light> createPointLight(glm::vec3 pos, double radius, double intensity, glm::vec3 color);
	static std::shared_ptr<Light> createSpotLight(glm::vec3 pos, glm::vec3 rotation, double radius, double intensity, glm::vec3 color);
	bool isBoxInView(Bounds bounds);
	static bool isBoxInListView(Bounds bounds, glm::mat4 modelMatrix, std::vector<std::shared_ptr<Light>> lights);

	//variables
	std::vector<glm::mat4> projectionMatrix;
	std::vector<glm::vec3> dir;
	std::vector<glm::vec3> dirComp;
	int numSubLights;
	double radius;
	double intensity;
	glm::vec3 color;
	Frustum viewFrustum;
	static glm::vec3 globalLightDirection;
	static double globalLightIntensity;
	static double ambient;
};


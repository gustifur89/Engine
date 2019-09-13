#pragma once
#include "Headers.h"
#include "Transform.h"

class Light : public Transform
{
private: 
	void addIndividualSpotLight(glm::vec3 pos, glm::vec3 direction, double radius, double intensity);

public:
	//methods
	Light();
	~Light();

	glm::mat4 getProjection(int index);

	static std::shared_ptr<Light> createPointLight(glm::vec3 pos, double radius, double intensity);
	static std::shared_ptr<Light> createSpotLight(glm::vec3 pos, glm::vec3 rotation, double radius, double intensity);

	//variables
	std::vector<glm::mat4> projectionMatrix;
	std::vector<glm::vec3> dir;
	double radius;
	double intensity;
};


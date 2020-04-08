#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "Camera.h"
#include "RenderTexture.h"
#include "Entity.h"

#define PORTAL_TEXTURE_SIZE 1024


class Portal : public GameObjectColor
{
private:
	double distanceToPlane(glm::vec3 entPos, glm::vec3 dir, glm::vec3 planePoint);
public:
	Portal();
	Portal(int width, int height);
	~Portal();

	void setWorld(std::shared_ptr<GameObject> world);
	void renderFunc(Camera& camera, glm::mat4 parentTransform);
	void portalRender(Camera camera, int width, int height);
	float getMinZ(Camera camera);
	void setRadius(float radius);
	void collideInternal(std::shared_ptr<Entity> entity);

	static void preRenderPortals(std::vector<std::shared_ptr<Portal>> portals, Camera& camera, int width, int height, int depth = 6);
	static void collide(std::vector<std::shared_ptr<Portal>> portals, std::shared_ptr<Entity> entity);
	static void collide(std::vector<std::shared_ptr<Portal>> portals, std::vector<std::shared_ptr<Entity>> entities);
	static void linkPortals(std::shared_ptr<Portal> portal1, std::shared_ptr<Portal> portal2);
	static void setUpShader(std::shared_ptr<ColorShader> shader);
	//parameters
	std::shared_ptr<Portal> otherPortal;
	std::shared_ptr<GameObject> world;
	static std::vector<Portal*> allPortals;
	RenderTexture renderTexture;
	bool updated;
	float radius;
};


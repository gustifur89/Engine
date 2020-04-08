#pragma once
#include "CollisionStructure.h"
#include "UIManager.h"
#include "Settings.h"
#include "PhysicsBody.h"
#include "Toggle.h"

class Entity : public GameObjectColor, public PhysicsBody
{
private:
	std::shared_ptr<Triangle> getFace(std::vector<std::shared_ptr<Triangle>> faces);
	double dotFace(std::shared_ptr<Triangle> face);
	std::vector<std::shared_ptr<Triangle>> getColliableFaces(std::vector<std::shared_ptr<Triangle>> faces);
public:
	enum LOOK_MODE
	{
		FPS,
		FLY
	};
	Entity(float radius, std::shared_ptr<Mesh> mesh, UIManager * UI, bool slidable = false);
	~Entity();

	void move(std::shared_ptr<CollisionStructure> collisionStructure);
	void move();
	void renderFunc(Camera& camera, glm::mat4 parentTransform);
	void updateCamera(Camera & camera, double upPercent);
	static void entityBounds(std::vector<std::shared_ptr<Entity>> entities);
	
	void setScale(glm::vec3 scale);
	glm::vec3 getScale();

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();

	void setRotation(glm::vec3 rotation);
	glm::vec3 getRotation();

	void setTransform(Transform transform);
	Transform getTransform();
	

	UIManager * UI;
	glm::vec3 vel;
	glm::vec3 lookRotation;
	bool colliding;
	bool floorColliding;
	bool roofColliding;
	bool portalCollide;
	bool slidable;
	int testName;
	float radius;
	LOOK_MODE looking;
};

class Holdable : public Entity
{
public:
	Holdable(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI, bool slidable = false);
	~Holdable();
	void move(std::shared_ptr<CollisionStructure> collisionStructure);

	void setTransform(Transform transform);

	bool beingHeld;
	bool onGround;
	Transform relative;
	static void move(std::vector<std::shared_ptr<Holdable>> items, std::shared_ptr<CollisionStructure> collisionStructure);
};

class Player : public Entity
{
private:
	void interact(std::shared_ptr<Holdable> items);
public:
	Player(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI);
	void move(std::shared_ptr<CollisionStructure> collisionStructure);
	void move();
	void setJump(double jump);
	void setJumpTime(double jumpTime);
	void interact(std::vector<std::shared_ptr<Holdable>> items);

	Camera * camera;
	bool noClip;
	bool noClipToggle;
	double jump;
	bool hasJump;
	Toggle jumpToggle;
	double jumpTime;
	double jumpTimeMax;
	double onGroundTime;
	std::shared_ptr<Holdable> held;
};

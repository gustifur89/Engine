#pragma once
#include "CollisionStructure.h"
#include "UIManager.h"
#include "Settings.h"

class Entity : public GameObjectColor
{
private:
	std::shared_ptr<Triangle> getFace(std::vector<std::shared_ptr<Triangle>> faces);
	double dotFace(std::shared_ptr<Triangle> face);
	std::vector<std::shared_ptr<Triangle>> getColliableFaces(std::vector<std::shared_ptr<Triangle>> faces);
public:
	Entity(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI, bool slidable = false);
	~Entity();

	void move(std::shared_ptr<CollisionStructure> collisionStructure);
	void updateCamera(Camera & camera, double upPercent);
	static void entityBounds(std::vector<std::shared_ptr<Entity>> entities);
	UIManager * UI;
	glm::vec3 vel;
	glm::vec3 lookRotation;
	Collider collider;
	bool colliding;
	bool floorColliding;
	bool roofColliding;
	bool portalCollide;
	bool slidable;
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
	void setJump(double jump);
	void setJumpTime(double jumpTime);
	void interact(std::vector<std::shared_ptr<Holdable>> items);

	Camera * camera;
	bool noClip;
	bool noClipToggle;
	double jump;
	bool hasJump;
	double jumpTime;
	double jumpTimeMax;
	double onGroundTime;
	std::shared_ptr<Holdable> held;
};

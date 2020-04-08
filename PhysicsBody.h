#pragma once
#include "Headers.h"
#include "CollisionDeclaration.h"
#include "CollisionStructure.h"
#include "Transform.h"


class PhysicsBody : public std::enable_shared_from_this<PhysicsBody>
{
private:

public:
//	std::shared_ptr<PhysicsBody> self;
	Transform transform;
	glm::vec3 forcePoint;
	glm::vec3 velocity;
	glm::vec3 impulsePoint;
	glm::vec3 impulse;
	glm::vec3 force;
	
	bool locked;
	float friction;
	float elasticity;

	std::vector<std::shared_ptr<Collider>> colliders;
	PhysicsBody();
	~PhysicsBody();

	//appliers
	void addCollider(glm::vec3 relPos, float radius);

	void applyForce(glm::vec3 force, glm::vec3 point);
	void applyImpulse(glm::vec3 force, glm::vec3 point);

	void collisionReaction(Collision collision);

	//methods
	void physicsIteration(float deltaTime);
};

class Physics
{
private:
	static void physicsStep(float deltaTime);
public:
	static float smallestVelocity;
	static glm::vec3 gravity;
	static float maxVelocity;

	static void physicsStep(float deltaTime, int steps);
	static void addCollider(std::shared_ptr<Collider> collider);
	static void addCollisionStructure(std::shared_ptr<CollisionStructure> collisionStructure);
	static void addPhysicsBody(std::shared_ptr<PhysicsBody> physicsBody);
	static std::vector<std::shared_ptr<PhysicsBody>> physicsBodyList;
	static std::vector<std::shared_ptr<Collider>> colliderList;
	static std::vector<std::shared_ptr<CollisionStructure>> collisionStructureList;
	static std::stack<Collision> collisions;
};



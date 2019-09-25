#pragma once
#include "Headers.h"
#include "CollisionStructure.h"


class PhysicsBody
{
private:

public:
	PhysicsBody();
	~PhysicsBody();


	std::vector<std::shared_ptr<Collider>> colliders;

	static std::vector<std::shared_ptr<PhysicsBody>> physicsBodyList;
	static std::vector<std::shared_ptr<Collider>> colliderList;
	static void physicsIteration();
};


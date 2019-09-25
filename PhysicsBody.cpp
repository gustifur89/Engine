#include "PhysicsBody.h"



// ============================ PhysicsBody ======================

std::vector<std::shared_ptr<PhysicsBody>> PhysicsBody::physicsBodyList = std::vector<std::shared_ptr<PhysicsBody>>();
std::vector<std::shared_ptr<Collider>> PhysicsBody::colliderList = std::vector<std::shared_ptr<Collider>>();

PhysicsBody::PhysicsBody()
{
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::physicsIteration()
{
	//PhysicsBodies have two parts: PhysicsBody and a list of associated colliders.
	//	Collider's need not have an assicoiated PhysicsBody (ground, static objects)

	//Put's all the colliders into an accelerating structure (OCTtree, equipartition octree, &c.)

	//Goes through each leaf node of the structure and collides all the PhysicsBodies' colliders (dynamic) against all colliders (static and dynamic)
	//	adding the collision to each of the colliders PhysicsBodies
	
	//goes through each PhysicsBody and calculates the net torque and force.
	//	applies the forces
	
	//updates position of all PhysicsBodies
}

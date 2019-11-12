#include "PhysicsBody.h"



// ============================ PhysicsBody ======================

PhysicsBody::PhysicsBody()
{
	//self = std::shared_ptr<PhysicsBody>(this);
	friction = 0.0;
	locked = false;
	elasticity = 0.0;
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::addCollider(glm::vec3 relPos, float radius)
{
	std::shared_ptr<Collider> collider(new Collider(radius));
	collider->physicsBody = shared_from_this();
	collider->relPos = glm::vec4(relPos, 1.0);
	this->colliders.push_back(collider);
	Physics::colliderList.push_back(collider);
}

void PhysicsBody::applyForce(glm::vec3 force, glm::vec3 point)
{
	//Get point reletive to center of the object
	//you give the point relative to the object's reference frame. Transform to world space for physics.
	glm::vec3 relPoint = glm::inverse(transform.getTransform()) * glm::vec4(point, 1.0);

	//recalculate the weighted center of the force application
	this->forcePoint += (this->force * this->forcePoint + force * relPoint) / (this->force + force);
	this->force += force;
}

void PhysicsBody::applyImpulse(glm::vec3 impulse, glm::vec3 point)
{
	//Get point reletive to center of the object
	//you give the point relative to the object's reference frame. Transform to world space for physics.
//	glm::vec3 relPoint = glm::inverse(transform.getTransform()) * glm::vec4(point, 1.0);

	//recalculate the weighted center of the force application
//	this->impulsePoint += (this->impulse * this->impulsePoint + impulse * relPoint) / (this->impulse + impulse);
	this->impulse += impulse;
}

void PhysicsBody::collisionReaction(Collision collision)
{
	float dot = glm::dot(collision.normal, collision.relVelocity);

	glm::vec3 projVelocity = dot < 0 ? -dot * collision.normal : glm::vec3(0);

	glm::vec3 rectifyingVector = collision.rectifyingDistance * collision.normal;

	float elasticFactor = 1.0 + this->elasticity; // 1.0 + this->elasticity;

	//collision.lockedCorrection *
	//collision.lockedCorrection 
	//collision.lockedCorrection   elasticFactor *
	//impulse does two things: apply an impulse to cancel out the velocity into the object. Also, apply an impulse to push them apart.
	
	//glm::vec3 impulse = collision.lockedCorrection * (-elasticFactor * projVelocity + rectifyingVector);
	glm::vec3 impulse = elasticFactor * projVelocity + collision.lockedCorrection * rectifyingVector;

	//maybe do a conservation of energy check?s

	//float currentEnergy = glm::length(this->velocity);
	//float nextEnergy = glm::length(this->velocity + impulse);
	//if (nextEnergy > currentEnergy)
	//{
	//	//energy violation
	//	//need to reduce impulse, but how?
	//	return;
	//}

	this->applyImpulse(impulse, collision.position);
}

void PhysicsBody::physicsIteration(float deltaTime)
{
	//PhysicsBodies have two parts: PhysicsBody and a list of associated colliders.
	//	Collider's need not have an assicoiated PhysicsBody (ground, static objects)

	//Put's all the colliders into an accelerating structure (OCTtree, equipartition octree, &c.)

	//Goes through each leaf node of the structure and collides all the PhysicsBodies' colliders (dynamic) against all colliders (static and dynamic)
	//	adding the collision to each of the colliders PhysicsBodies
	
	//goes through each PhysicsBody and calculates the net torque and force.
	//	applies the forces
	
	//updates position of all PhysicsBodies

	if (velocity != glm::vec3(0))
	{
		glm::vec3 frictionForce = -friction * glm::normalize(velocity);
		this->applyForce(frictionForce, glm::vec3(0));
	}
	velocity += impulse + deltaTime * force;
	impulse = glm::vec3(0);
	force = glm::vec3(0);
	forcePoint = glm::vec3(0);
	impulsePoint = glm::vec3(0);
	
	//if (fabs(velocity.x) <= Physics::smallestVelocity - deltaTime * fabs(Physics::gravity.x)) velocity.x = 0.0;
	//if (fabs(velocity.y) <= Physics::smallestVelocity - deltaTime * fabs(Physics::gravity.y)) velocity.y = 0.0;
	//if (fabs(velocity.z) <= Physics::smallestVelocity - deltaTime * fabs(Physics::gravity.z)) velocity.z = 0.0;
	
	if (glm::length(velocity) >= Physics::maxVelocity) velocity = Physics::maxVelocity * glm::normalize(velocity);

	transform.position += deltaTime * velocity;
}

//====================== Physics ========================

float Physics::smallestVelocity = 0.1;
float Physics::maxVelocity = 100.0;
glm::vec3 Physics::gravity = glm::vec3(0);
std::vector<std::shared_ptr<PhysicsBody>> Physics::physicsBodyList = std::vector<std::shared_ptr<PhysicsBody>>();
std::vector<std::shared_ptr<Collider>> Physics::colliderList = std::vector<std::shared_ptr<Collider>>();
std::vector<std::shared_ptr<CollisionStructure>> Physics::collisionStructureList = std::vector<std::shared_ptr<CollisionStructure>>();
std::stack<Collision> Physics::collisions = std::stack<Collision>();

void Physics::physicsStep(float deltaTime, int steps)
{
	if (steps < 1) steps = 1;
	deltaTime /= (float)steps;
	for (int i = 0; i < steps; i++)
	{
		Physics::physicsStep(deltaTime);
	}
}

void Physics::physicsStep(float deltaTime)
{
	//Collider collisions
	//update object's colliders
	for (int i = 0; i < physicsBodyList.size(); i++)
	{
		for (int j = 0; j < physicsBodyList[i]->colliders.size(); j++)
		{
			physicsBodyList[i]->colliders[j]->update(physicsBodyList[i]->transform.getTransform(), physicsBodyList[i]->velocity, physicsBodyList[i]->locked);
		}
	}
	

	//Collide everything
	/*
	for (int i = 0; i < colliderList.size(); i++)
	{
		for (int j = i+1; j < colliderList.size(); j++)
		{
			colliderList[i]->collision(colliderList[j], Physics::collisions);
		}
	}
	*/
	for (int i = 0; i < physicsBodyList.size(); i++)
	{
		for (int j = 0; j < physicsBodyList[i]->colliders.size(); j++)
		{
			for (int k = 0; k < colliderList.size(); k++)
			{
				if(colliderList[k]->physicsBody != physicsBodyList[i])
					physicsBodyList[i]->colliders[j]->collision(colliderList[k], collisions);
			}
			for (int k = 0; k < collisionStructureList.size(); k++)
			{
				physicsBodyList[i]->colliders[j]->collision(collisionStructureList[k], collisions);
			}
		}
	}



	//then react. May just add to the collide everything step;
	
	while (collisions.size() > 0)
	{
		Collision collision = collisions.top(); 
		collisions.pop();
		collision.physicsBody->collisionReaction(collision);
	}
	//*/

	for (int i = 0; i < physicsBodyList.size(); i++)
	{
		physicsBodyList[i]->applyForce(gravity, glm::vec3(0,0,0));
		physicsBodyList[i]->physicsIteration(deltaTime);
	}

	//collider everything
	/*
	//then update the physics
	if (physBody->transform.position.y <= 0)
	{
		//get component of velocity parallel to the normal
		glm::vec3 fNorm(0, 1, 0);
		float dot = glm::dot(fNorm, physBody->velocity);
		glm::vec3 projVelocity = dot * fNorm;
		//impulse should cancel the velocity into the object and offset back out of object
		glm::vec3 rectifyingDistance = glm::dot(fNorm, physBody->transform.position) * fNorm;
		glm::vec3 test(0, -physBody->transform.position.y, 0);
		physBody->applyImpulse(((dot < 0) ? -projVelocity : glm::vec3(0)) - rectifyingDistance, glm::vec3(0, 0, 0));
	}

	physBody->physicsIteration(UI.deltaTime);
	physBlock->transform = physBody->transform;
	*/
}

void Physics::addCollider(std::shared_ptr<Collider> collider)
{
	colliderList.push_back(collider);
}

void Physics::addCollisionStructure(std::shared_ptr<CollisionStructure> collisionStructure)
{
	collisionStructureList.push_back(collisionStructure);
}

void Physics::addPhysicsBody(std::shared_ptr<PhysicsBody> physicsBody)
{
	physicsBodyList.push_back(physicsBody);
}

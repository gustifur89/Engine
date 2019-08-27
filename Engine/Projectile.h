#pragma once
#include "Headers.h"
#include "Entity.h"

class Projectile : public Entity
{
public:
	glm::vec3 direction;
	double speed;
	bool collided;


	Projectile(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI, glm::vec3 direction, double speed);
	~Projectile();

	void move(std::shared_ptr<CollisionStructure> collisionStructure);

	virtual void collide();

	static std::shared_ptr<Projectile> makeProjectile(double raidus, std::shared_ptr<Mesh> mesh, UIManager * UI, glm::vec3 position, glm::vec3 direction, double speed);

	static std::vector<std::shared_ptr<Projectile>> projectiles;
};


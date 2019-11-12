#include "Projectile.h"



Projectile::Projectile(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI, glm::vec3 direction, double speed) : Entity(radius, mesh, UI)
{
	this->direction = direction;
	this->speed = speed;
	collided = false;
	velocity = (float) speed* direction;
}

std::vector<std::shared_ptr<Projectile>> Projectile::projectiles = std::vector<std::shared_ptr<Projectile>>();

std::shared_ptr<Projectile> Projectile::makeProjectile(double raidus, std::shared_ptr<Mesh> mesh, UIManager * UI, glm::vec3 position, glm::vec3 direction, double speed)
{
	std::shared_ptr<Projectile> out = std::shared_ptr<Projectile>(new Projectile(raidus, mesh, UI, direction, speed));
	out->setPosition(position);
	out->setScale(glm::vec3(raidus));
	projectiles.push_back(out);
	return out;
}

void Projectile::move(std::shared_ptr<CollisionStructure> collisionStructure)
{
	if (!collided)
	{
		vel.y += Settings::gravity * UI->deltaTime;
		Entity::move(collisionStructure);
		if (colliding)
		{
			this->collide();
		}
	}
}

void Projectile::collide()
{
//	std::cout << "proj\n";
	vel.x = 0;
	vel.y = 0;
	vel.z = 0;
	collided = true;
}

Projectile::~Projectile()
{

}

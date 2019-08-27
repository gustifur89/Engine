#include "Entity.h"

// ================================== Entity ===========================================

Entity::Entity(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI, bool slidable)
{
	collider.radius = radius;
	collider.x = &this->transform.x;
	collider.y = &this->transform.y;
	collider.z = &this->transform.z;
	collider.vel = &this->vel;
	this->mesh = mesh;
	lookRotation = glm::vec3(0.0);
	this->UI = UI;
	this->slidable = slidable;
}

Entity::~Entity()
{
}

double Entity::dotFace(std::shared_ptr<Triangle> face)
{
	return face->normal.x * vel.x + face->normal.y * vel.y + face->normal.z * vel.z;
}

std::vector<std::shared_ptr<Triangle>> Entity::getColliableFaces(std::vector<std::shared_ptr<Triangle>> faces)
{
	std::vector<std::shared_ptr<Triangle>> out;
	for (int i = 0; i < faces.size(); i++)
	{
		if (dotFace(faces[i]) < 0)
		{
			out.push_back(faces[i]);
		}
	}
	return out;
}

std::shared_ptr<Triangle> Entity::getFace(std::vector<std::shared_ptr<Triangle>> faces)
{
	double maxAbsDot = fabs(dotFace(faces[0]));
	std::shared_ptr<Triangle> face = faces[0];
	for (int i = 1; i < faces.size(); i++)
	{
		double absDot = fabs(dotFace(faces[i]));
		if (absDot > maxAbsDot)
		{
			maxAbsDot = absDot;
			face = faces[i];
		}
	}
	return face;
}

void Entity::move(std::shared_ptr<CollisionStructure> collisionStructure)
{
	colliding = false;
	floorColliding = false;
//	if (glm::length(vel) > Settings::maxSpeed)
//	{
//		vel = ((float) Settings::maxSpeed / glm::length(vel)) * glm::normalize(vel);
//	}

	if (glm::length(vel) == 0.0) return;

	int steps = ceil((UI->deltaTime * glm::length(vel)) / Settings::minHitStep);
	 
	//glm::vec3 fracMove = (1.f / Settings::hitResolution) * (float)UI->deltaTime * vel;
	glm::vec3 fracMove = (1.f / steps) * (float)UI->deltaTime * vel;

	for (int i = 0; i < steps; i++)
	{
		transform.x += fracMove.x;
		transform.y += fracMove.y;
		transform.z += fracMove.z;
		if (collisionStructure == nullptr) continue;
		std::vector<std::shared_ptr<Triangle>> faces = collisionStructure->collide(&collider);
		faces = getColliableFaces(faces);
	//	std::cout << faces.size() << "\n";
		if (faces.size() > 0)
		{
			colliding = true;
			transform.x -= fracMove.x;
			transform.y -= fracMove.y;
			transform.z -= fracMove.z;
			if (slidable)
			{
				std::shared_ptr<Triangle> face = getFace(faces);
				if (face->normal.y > 0.8) floorColliding = true;
				if (face->normal.y < -0.8) roofColliding = true;
				//move back a little
				transform.x += face->normal.x * Settings::epsilon;
				transform.y += face->normal.y * Settings::epsilon;
				transform.z += face->normal.z * Settings::epsilon;

				//Get perpedicular
				glm::vec3 moveRest = ((float)steps - i) * fracMove;
				glm::vec3 perp = moveRest - (glm::dot(moveRest, face->normal) / glm::length(face->normal)) * face->normal;

				transform.x += perp.x;
				transform.y += perp.y;
				transform.z += perp.z;
				faces = collisionStructure->collide(&collider);
				faces = getColliableFaces(faces);
				if (faces.size() > 0)
				{
					transform.x -= perp.x;
					transform.y -= perp.y;
					transform.z -= perp.z;
				}

			}
			return;
		}
		
	}
}

void Entity::updateCamera(Camera & camera, double upPercent)
{
	camera.angleX = lookRotation.x;
	camera.angleY = lookRotation.y;
	camera.angleZ = lookRotation.z;
	camera.x = transform.x;
	camera.y = transform.y + collider.radius * upPercent;
	camera.z = transform.z;
}

void Entity::entityBounds(std::vector<std::shared_ptr<Entity>> entities)
{
	for (std::shared_ptr<Entity> &ent : entities)
	{
		//if () {

	//	}
	}
}


// ================================ Holdabale ========================


Holdable::Holdable(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI, bool slidable) : Entity(radius, mesh, UI, slidable)
{
	beingHeld = false;
	onGround = false;
	vel.y = 0.0;
}

Holdable::~Holdable()
{
}

void Holdable::move(std::shared_ptr<CollisionStructure> collisionStructure)
{
	if (!onGround)
	{
		vel.y += Settings::gravity * UI->deltaTime;
		if (floorColliding)
		{
			vel.y = 0.0;
			onGround = true;
		}
		if (roofColliding)
		{
			vel.y = 0.0;
		}

		Entity::move(collisionStructure);
	}
}

void Holdable::setTransform(Transform transform)
{
	glm::vec3 angles = glm::vec3(transform.angleX, transform.angleY, transform.angleZ);

	glm::vec3 Z = Transform::getTransformedZ(angles);
	glm::vec3 X = Transform::getTransformedX(angles);
	glm::vec3 Y = Transform::getTransformedY(angles);

	this->transform.x = transform.x + X.x * relative.x + Y.x * relative.y + Z.x * relative.z;
	this->transform.y = transform.y + X.y * relative.x + Y.y * relative.y + Z.y * relative.z;
	this->transform.z = transform.z + X.z * relative.x + Y.z * relative.y + Z.z * relative.z;

	this->transform.angleX = transform.angleX + relative.angleX;
	this->transform.angleY = transform.angleY + relative.angleY;
	this->transform.angleZ = transform.angleZ + relative.angleZ;
}

void Holdable::move(std::vector<std::shared_ptr<Holdable>> items, std::shared_ptr<CollisionStructure> collisionStructure)
{
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->move(collisionStructure);
	}
}


// ================================= Player ==============================================


Player::Player(double radius, std::shared_ptr<Mesh> mesh, UIManager * UI) : Entity(radius, mesh, UI, true)
{
	noClipToggle = true;
	noClip = true;
}

void Player::setJump(double jump)
{
	this->jump = jump;
}

void Player::setJumpTime(double jumpTime)
{
	this->jumpTimeMax = jumpTime;
}

void Player::interact(std::shared_ptr<Holdable> item)
{
	bool e = UI->isKeyPressed(GLFW_KEY_E);
	bool q = UI->isKeyPressed(GLFW_KEY_Q);
	if (e || q)
	{
		glm::vec3 pPos = glm::vec3(*collider.x, *collider.y, *collider.z);
		glm::vec3 iPos = glm::vec3(*item->collider.x, *item->collider.y, *item->collider.z);
		if (glm::length(pPos - iPos) <= collider.radius + item->collider.radius)
		{
			if (e)
			{
				//pick up

			}
			if(q)
			{
				//drop

			}
		}
	}
}

void Player::interact(std::vector<std::shared_ptr<Holdable>> items)
{
	for (int i = 0; i < items.size(); i++)
	{
		this->interact(items[i]);
	}
}

void Player::move(std::shared_ptr<CollisionStructure> collisionStructure)
{
	if (UI->isKeyPressed(GLFW_KEY_V))
	{
		if (noClipToggle)
		{
			noClip = !noClip;
		}
		noClipToggle = false;
	}
	else
	{
		noClipToggle = true;
	}

	bool w = UI->isKeyPressed(GLFW_KEY_W);
	bool s = UI->isKeyPressed(GLFW_KEY_S);
	bool a = UI->isKeyPressed(GLFW_KEY_A);
	bool d = UI->isKeyPressed(GLFW_KEY_D);

	bool e = UI->isKeyPressed(GLFW_KEY_SPACE);
	bool q = UI->isKeyPressed(GLFW_KEY_LEFT_CONTROL);

	bool up = UI->isKeyPressed(GLFW_KEY_UP);
	bool down = UI->isKeyPressed(GLFW_KEY_DOWN);
	bool right = UI->isKeyPressed(GLFW_KEY_RIGHT);
	bool left = UI->isKeyPressed(GLFW_KEY_LEFT);

	double forwardSpeed = 0.0;
	double strafeSpeed = 0.0;
	double verticalSpeed = 0.0;

	//MOVEMENT
	if (w == s)
		forwardSpeed = 0.0;
	else if (w)
		forwardSpeed = 10.0;// *UI->deltaTime;
	else
		forwardSpeed = -10.0;// *UI->deltaTime;

	if (a == d)
		strafeSpeed = 0.0;
	else if (a)
		strafeSpeed = 10.0;// *UI->deltaTime;
	else
		strafeSpeed = -10.0;// *UI->deltaTime;

	if (e == q)
		verticalSpeed = 0.0;
	else if (q)
		verticalSpeed = -10.0;// *UI->deltaTime;
	else
		verticalSpeed = 10.0;// *UI->deltaTime;

	if (UI->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		forwardSpeed *= 2.0;
		strafeSpeed *= 2.0;
		verticalSpeed *= 2.0;
	}

	//LOOK
	if (UI->getMouseLockState()) {
		double deltaPitch = UI->deltaMouseY * Settings::mouseSensitivity;
		double deltaPivot = -UI->deltaMouseX * Settings::mouseSensitivity;
		lookRotation.x += deltaPitch;
		lookRotation.y += deltaPivot;
		if (lookRotation.x > 89.9) lookRotation.x = 89.9;
		if (lookRotation.x < -89.9) lookRotation.x = -89.9;
	}

	glm::vec3 directionForward = Transform::getTransformedZ(lookRotation);
	glm::vec3 directionStrafe = Transform::getTransformedX(lookRotation);
	glm::vec3 directionVertical = Transform::getTransformedY(lookRotation);

	if (noClip)
	{
		vel.x = directionForward.x * forwardSpeed + directionStrafe.x * strafeSpeed + directionVertical.x * verticalSpeed;
		vel.y = directionForward.y * forwardSpeed + directionStrafe.y * strafeSpeed + directionVertical.y * verticalSpeed;
		vel.z = directionForward.z * forwardSpeed + directionStrafe.z * strafeSpeed + directionVertical.z * verticalSpeed;
	}
	else
	{

		vel.y += Settings::gravity * UI->deltaTime;
		if (floorColliding)
		{
			onGroundTime = 0.0;
			hasJump = true;
		}
		if (roofColliding)
		{
			vel.y = 0.0;
		}

		if (hasJump && onGroundTime <= Settings::onGroundTimeMax)
		{
			vel.y = 0.0;
			onGroundTime += UI->deltaTime;
		}

		if (UI->isKeyPressed(GLFW_KEY_SPACE))
		{
			if(hasJump)
				jumpTime = 0.0;
			hasJump = false;
			if (jumpTime <= jumpTimeMax)
			{
				jumpTime += UI->deltaTime;
				vel.y = jump;
			}
		}
		else
		{
			jumpTime = jumpTimeMax + 1.0;
		}
		

		vel.z = cos(lookRotation.y * TO_RAD) * forwardSpeed - sin(lookRotation.y * TO_RAD) * strafeSpeed;
		vel.x = sin(lookRotation.y * TO_RAD) * forwardSpeed + cos(lookRotation.y * TO_RAD) * strafeSpeed;
	}

	//std::cout << collisionStructure.collide(collider).size() << "\n";

	if (UI->isKeyPressed(GLFW_KEY_3))
	{
		transform.x = 10;
		transform.y = 10;
		transform.z = 10;
	}

	Entity::move(collisionStructure);

	if (held != nullptr)
	{
		held->setTransform(transform);
	}
}

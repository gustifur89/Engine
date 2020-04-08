#include "Entity.h"

// ================================== Entity ===========================================

Entity::Entity(float radius, std::shared_ptr<Mesh> mesh, UIManager * UI, bool slidable)
{
//	collider = std::shared_ptr<Collider>(new Collider(radius));
//	collider->transform.position = this->transform.position;
//	collider->velocity = this->vel;
	this->mesh = mesh;
	lookRotation = glm::vec3(0.0);
	this->UI = UI;
	this->slidable = slidable;
	this->testName = 0;
	this->radius = radius;
}


Entity::~Entity()
{
	std::cout << "bye entity\n";
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
	/*  DEFUNCT
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
		transform.position += fracMove;
		if (collisionStructure == nullptr) continue;
		std::vector<std::shared_ptr<Triangle>> faces = collisionStructure->collide(collider);
		faces = getColliableFaces(faces);
	//	std::cout << faces.size() << "\n";
		if (faces.size() > 0)
		{
			colliding = true;
			transform.position -= fracMove;
			if (slidable)
			{
				std::shared_ptr<Triangle> face = getFace(faces);
				if (face->normal.y > 0.8) floorColliding = true;
				if (face->normal.y < -0.8) roofColliding = true;
				//move back a little
				transform.position += (float) Settings::epsilon * face->normal;

				//Get perpedicular
				glm::vec3 moveRest = ((float)steps - i) * fracMove;
				glm::vec3 perp = moveRest - (glm::dot(moveRest, face->normal) / glm::length(face->normal)) * face->normal;

				transform.position += perp;
				faces = collisionStructure->collide(collider);
				faces = getColliableFaces(faces);
				if (faces.size() > 0)
				{
					transform.position -= perp;
				}

			}
			return;
		}
		
	}
	*/
}

void Entity::updateCamera(Camera & camera, double upPercent)
{
	camera.setRotation(PhysicsBody::transform);
	camera.position = PhysicsBody::transform.position;
	//camera.position.y += collider->radius * upPercent;
}

void Entity::entityBounds(std::vector<std::shared_ptr<Entity>> entities)
{
	for (std::shared_ptr<Entity> &ent : entities)
	{
		//if () {

	//	}
	}
}

void Entity::setScale(glm::vec3 scale)
{
	PhysicsBody::transform.scale = scale;
	GameObjectColor::transform.scale = scale;
}

glm::vec3 Entity::getScale()
{
	return PhysicsBody::transform.scale;
}

void Entity::setPosition(glm::vec3 position)
{
	PhysicsBody::transform.position = position;
	GameObjectColor::transform.position = position;
}

glm::vec3 Entity::getPosition()
{
	return PhysicsBody::transform.position;
}

void Entity::setRotation(glm::vec3 rotation)
{
	PhysicsBody::transform.setRotation(rotation);
	GameObjectColor::transform.setRotation(rotation);
}

glm::vec3 Entity::getRotation()
{
	return PhysicsBody::transform.getEulerAngles();
}

void Entity::setTransform(Transform transform)
{
	PhysicsBody::transform = transform;
	GameObjectColor::transform = transform;
}

Transform Entity::getTransform()
{
	return PhysicsBody::transform;
}

void Entity::move()
{
	GameObject::transform = PhysicsBody::transform;
}

void Entity::renderFunc(Camera& camera, glm::mat4 parentTransform)
{
	GameObject::transform = PhysicsBody::transform;
	GameObjectColor::renderFunc(camera, parentTransform);
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
	glm::vec3 Z = transform.getTransformedZ();
	glm::vec3 X = transform.getTransformedX();
	glm::vec3 Y = transform.getTransformedY();


//	glm::mat3 POS = glm::mat3(X,Y,Z);

	this->PhysicsBody::transform.position.x = transform.position.x + X.x * relative.position.x + Y.x * relative.position.y + Z.x * relative.position.z;
	this->PhysicsBody::transform.position.y = transform.position.y + X.y * relative.position.x + Y.y * relative.position.y + Z.y * relative.position.z;
	this->PhysicsBody::transform.position.z = transform.position.z + X.z * relative.position.x + Y.z * relative.position.y + Z.z * relative.position.z;

	this->PhysicsBody::transform.rotate(relative.getEulerAngles());
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
	looking = LOOK_MODE::FPS;
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
		if (glm::length(PhysicsBody::transform.position - item->PhysicsBody::transform.position) <= this->radius + item->radius)
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

	if (UI->isKeyPressed(GLFW_KEY_1))
	{
		this->floorColliding = false;
		this->roofColliding = false;
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
		vel = (float) forwardSpeed * directionForward + (float) strafeSpeed * directionStrafe + (float) verticalSpeed * directionVertical;

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
		PhysicsBody::transform.position = glm::vec3(10.0);
	}

	Entity::move(collisionStructure);

	if (held != nullptr)
	{
		held->setTransform(PhysicsBody::transform);
	}

	//update for rendering
	GameObject::transform = PhysicsBody::transform;
}

void Player::move()
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

	bool space = UI->isKeyPressed(GLFW_KEY_SPACE);
	bool ctrl = UI->isKeyPressed(GLFW_KEY_LEFT_CONTROL);

	bool up = UI->isKeyPressed(GLFW_KEY_UP);
	bool down = UI->isKeyPressed(GLFW_KEY_DOWN);
	bool right = UI->isKeyPressed(GLFW_KEY_RIGHT);
	bool left = UI->isKeyPressed(GLFW_KEY_LEFT);

	bool e = UI->isKeyPressed(GLFW_KEY_E);
	bool q = UI->isKeyPressed(GLFW_KEY_Q);

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

	if (space == ctrl)
		verticalSpeed = 0.0;
	else if (ctrl)
		verticalSpeed = -10.0;// *UI->deltaTime;
	else
		verticalSpeed = 10.0;// *UI->deltaTime;

	if (UI->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		forwardSpeed *= 2.0;
		strafeSpeed *= 2.0;
		verticalSpeed *= 2.0;
	}

	if (UI->isKeyPressed(GLFW_KEY_1))
	{
		this->floorColliding = false;
		this->roofColliding = false;
	}

	//LOOK

	if (looking == LOOK_MODE::FPS)
	{
		if (UI->getMouseLockState()) {
			double deltaPitch = UI->deltaMouseY * Settings::mouseSensitivity;
			double deltaPivot = -UI->deltaMouseX * Settings::mouseSensitivity;
			lookRotation.x += deltaPitch;
			lookRotation.y += deltaPivot;
			if (lookRotation.x > 89.9) lookRotation.x = 89.9;
			if (lookRotation.x < -89.9) lookRotation.x = -89.9;
		}

		PhysicsBody::transform.setRotation(lookRotation);
	}
	else if (looking == LOOK_MODE::FLY)
	{
		glm::vec3 dRotation = glm::vec3(0, 0, 0);;

		if (UI->getMouseLockState()) {
			double deltaPitch = UI->deltaMouseY * Settings::mouseSensitivity;
			double deltaPivot = -UI->deltaMouseX * Settings::mouseSensitivity;
			//lookRotation.x += deltaPitch;
			//lookRotation.y += deltaPivot;
			dRotation.x = deltaPitch;
			dRotation.y = deltaPivot;
			//if (lookRotation.x > 89.9) lookRotation.x = 89.9;
			//if (lookRotation.x < -89.9) lookRotation.x = -89.9;
		}


		double rollSpeed = 0.0;
		if (e == q)
			rollSpeed = 0.0;
		else if (q)
			rollSpeed = -80.0;
		else
			rollSpeed = 80.0;

		//lookRotation.z += (float) UI->deltaTime * rollSpeed;
		dRotation.z = (float)UI->deltaTime * rollSpeed;

		PhysicsBody::transform.rotate(dRotation);
	}

	glm::vec3 directionForward = PhysicsBody::transform.Transform::getTransformedZ();
	glm::vec3 directionStrafe = PhysicsBody::transform.Transform::getTransformedX();
	glm::vec3 directionVertical = PhysicsBody::transform.Transform::getTransformedY();

	//glm::vec3 directionForward = Transform::getTransformedZ(lookRotation);
	//glm::vec3 directionStrafe = Transform::getTransformedX(lookRotation);
	//glm::vec3 directionVertical = Transform::getTransformedY(lookRotation);

	if (noClip)
	{
		vel = (float)forwardSpeed * directionForward + (float)strafeSpeed * directionStrafe + (float)verticalSpeed * directionVertical;
		this->velocity = vel;
	}
	else
	{

		
		if (floorColliding)
		{
			onGroundTime = 0.0;
			hasJump = true;
		}
		if (roofColliding)
		{
			
		}

		if (hasJump && onGroundTime <= Settings::onGroundTimeMax)
		{
			
			onGroundTime += UI->deltaTime;
		}

		if (jumpToggle.toggle(UI->isKeyPressed(GLFW_KEY_SPACE)))
		{
			this->applyImpulse(glm::vec3(0, 10, 0), glm::vec3(0));
			if (hasJump)
				jumpTime = 0.0;
			hasJump = false;
			if (jumpTime <= jumpTimeMax)
			{
				jumpTime += UI->deltaTime;
			}
		}
		else
		{
			jumpTime = jumpTimeMax + 1.0;
		}


		vel.z = cos(lookRotation.y * TO_RAD) * forwardSpeed - sin(lookRotation.y * TO_RAD) * strafeSpeed;
		vel.x = sin(lookRotation.y * TO_RAD) * forwardSpeed + cos(lookRotation.y * TO_RAD) * strafeSpeed;

		this->velocity.x = vel.x;
		this->velocity.z = vel.z;
	}

	//std::cout << collisionStructure.collide(collider).size() << "\n";

	if (UI->isKeyPressed(GLFW_KEY_3))
	{
		PhysicsBody::transform.position = glm::vec3(0, 10.0, 0);
	}

	//Entity::move(collisionStructure);

	if (held != nullptr)
	{
		held->setTransform(PhysicsBody::transform);
	}

	GameObject::transform = PhysicsBody::transform;
}


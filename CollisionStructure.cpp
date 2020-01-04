#include "CollisionStructure.h"

//============================== Collider ==========================================

Collider::Collider() : Collider(0.0)
{

}

Collider::Collider(float radius)
{
//	self = std::shared_ptr<Collider>(this);
	physicsBody = std::shared_ptr<PhysicsBody>(NULL);
	locked = true;
	this->radius = radius;
	velocity = glm::vec3(0);
	colliderType = 0;
}

Collider::~Collider()
{
	//self = std::shared_ptr<Collider>(NULL);
}

void Collider::update(glm::mat4& localToWorld, glm::vec3& velocity, bool locked)
{
	this->transform.position = localToWorld * relPos; //physicsBody->transform.position + glm::vec3(relPos);
	this->velocity = velocity;
	this->locked = locked;
}

void Collider::collision(std::shared_ptr<Collider> collider, std::stack<Collision>& collisions)
{
	if (this->colliderType != 0 && this->colliderType == collider->colliderType) return;

	float distance = glm::length(this->transform.position - collider->transform.position);

	if (distance <= 0.001) return;

	if (distance <= this->radius + collider->radius && distance > fmin(this->radius, collider->radius))
	{
		//http://hyperphysics.phy-astr.gsu.edu/hbase/colsta.html
		float p1 = 0.0;
		float p2 = 0.0;
		
		if (collider->locked)
		{
			p1 = -1.0f;
		}
		else
		{
			p1 = (this->physicsBody->mass - collider->physicsBody->mass) / (this->physicsBody->mass + collider->physicsBody->mass);
			p2 = (2.0 * collider->physicsBody->mass) / (this->physicsBody->mass + collider->physicsBody->mass);
		}
		
		float rectify = (this->radius + collider->radius) - distance;

		glm::vec3 momentumCorrection = p1 * this->velocity + p2 * collider->velocity;

		//position is the center. Normal is the direction from the other ball to this ball
		glm::vec3 normal = glm::normalize(this->transform.position - collider->transform.position);
		glm::vec3 position = 0.5f * (this->transform.position + collider->transform.position);

		//get component of velocity parallel to the normal

		/*
		glm::vec3 diff = glm::vec3(0);// this->transform.position;//this->transform.position + this->radius * normal -collider->transform.position - this->radius * normal;


		float dot = glm::dot(normal, relVelocity);
		glm::vec3 projVelocity = dot < 0 ? dot * normal : glm::vec3(0);

		//impulse should cancel the velocity into the object and offset back out of object
		glm::vec3 rectifyingDistance = rectify * normal;
		*/
	//	this->physicsBody->applyImpulse(bothOffset * (-projVelocity + rectifyingDistance), glm::vec3(0, 0, 0));

		//Collision nCollision(position, normal, this->physicsBody);
		//nCollision.impulse = bothOffset * (-projVelocity + rectifyingDistance);
		//Collision nCollision(position, normal, relVelocity, rectify, momentumCorrection, this->physicsBody);
		Collision nCollision(position, normal, momentumCorrection, rectify, this->physicsBody);
		collisions.push(nCollision);

		//if(!this->locked)
		//	this->physicsBody->applyImpulse(bothOffset * (-projVelocity + -rectifyingDistance), glm::vec3(0, 0, 0));


		//if (!this->locked)
		//	collider->physicsBody->applyImpulse(bothOffset * (projVelocity + rectifyingDistance), glm::vec3(0, 0, 0));
	}
}

void Collider::collision(std::shared_ptr<Collider> collider)
{
	//forgo AABB test
	//just a simple sphere collision test:
	float distance = glm::length(this->transform.position - collider->transform.position);

	if (distance <= this->radius + collider->radius)
	{
		float bothOffset = (this->locked | collider->locked) ? 1.0f : 0.5f;

		float rectify = (this->radius + collider->radius) - distance;

		glm::vec3 relVelocity = this->velocity - collider->velocity;

		//position is the center. Normal is the direction from the other ball to this ball
		glm::vec3 normal = glm::normalize(this->transform.position - collider->transform.position);
		glm::vec3 position = 0.5f * (this->transform.position + collider->transform.position);

		//get component of velocity parallel to the normal

		glm::vec3 diff = glm::vec3(0);// this->transform.position;//this->transform.position + this->radius * normal -collider->transform.position - this->radius * normal;

		float dot = glm::dot(normal, relVelocity);
		glm::vec3 projVelocity = dot < 0 ? dot * normal : glm::vec3(0);

		//impulse should cancel the velocity into the object and offset back out of object
		glm::vec3 rectifyingDistance = rectify* normal;

		this->physicsBody->applyImpulse( bothOffset * (-projVelocity + rectifyingDistance), glm::vec3(0, 0, 0));

		//if(!this->locked)
		//	this->physicsBody->applyImpulse(bothOffset * (-projVelocity + -rectifyingDistance), glm::vec3(0, 0, 0));
		
		
		//if (!this->locked)
		//	collider->physicsBody->applyImpulse(bothOffset * (projVelocity + rectifyingDistance), glm::vec3(0, 0, 0));
	}
}

void Collider::collision(std::shared_ptr<CollisionStructure> collisionStructure, std::stack<Collision>& collisions)
{
	std::vector<std::shared_ptr<Triangle>> faces = collisionStructure->collide(shared_from_this());//self
	//std::cout << faces.size() << "\n";
	std::stack<Collision> tempStack;
	for (int i = 0; i < faces.size(); i++)
	{
		this->collision(faces[i], tempStack);
	}

	//maybe find the center of mass... 
	//then find the distance to center of mass...
	
	/*
	if (tempStack.size() > 0)
	{
		glm::vec3 relVel = tempStack.top().relVelocity;
		glm::vec3 pos(0);
		float size = 1.0f / tempStack.size();
		while (tempStack.size() > 0)
		{
			pos += tempStack.top().position;
			tempStack.pop();
		}

		pos = size * pos;

		glm::vec3 normal = this->transform.position - pos;
		float distance = this->radius - glm::length(normal);
		normal = glm::normalize(normal);

		Collision nCollision(pos, normal, relVel, distance, 1.0f, this->physicsBody);
		collisions.push(nCollision);
	}
	//*/

	//Now we need to do grahm-schmidt on the results to avoid double counting and stuff. It caused errors where balls would just bounce off of meshes.
	//There is only one true collision from a mesh, so we need to add the individual in a smart way (graham)
	//assume position is just the center of it.
	//relative velocity should be the same

	//We need to use the method on rectify * norm. That way it weights for other stuff. I think. Then split it back up.
	while (tempStack.size() > 0)
	{
		collisions.push(tempStack.top());
		tempStack.pop();
	}
	return;

	/*
	float distance = glm::length(this->transform.position - collider->transform.position);

	if (distance <= this->radius + collider->radius)
	{
		float bothOffset = (this->locked | collider->locked) ? 1.0f : 0.5f;

		float rectify = (this->radius + collider->radius) - distance;

		glm::vec3 relVelocity = this->velocity - collider->velocity;

		//position is the center. Normal is the direction from the other ball to this ball
		glm::vec3 normal = glm::normalize(this->transform.position - collider->transform.position);
		glm::vec3 position = 0.5f * (this->transform.position + collider->transform.position);

		//get component of velocity parallel to the normal

		glm::vec3 diff = glm::vec3(0);// this->transform.position;//this->transform.position + this->radius * normal -collider->transform.position - this->radius * normal;

		float dot = glm::dot(normal, relVelocity);
		glm::vec3 projVelocity = dot < 0 ? dot * normal : glm::vec3(0);

		//impulse should cancel the velocity into the object and offset back out of object
		glm::vec3 rectifyingDistance = rectify * normal;

		this->physicsBody->applyImpulse(bothOffset * (-projVelocity + rectifyingDistance), glm::vec3(0, 0, 0));
	}
	*/
}

void Collider::collision(std::shared_ptr<Triangle> triangle, std::stack<Collision>& collisions)
{
	/*
	if (true || OctTree::distanceToPlane(shared_from_this(), triangle))//distanceToTriangle
	{
		//see if close. collide off of it's normal;
		glm::vec3 relVelocity = this->velocity;

		float distance = OctTree::distanceToPlane(shared_from_this(), triangle);

		glm::vec3 normal = triangle->normal;

		glm::vec3 pos = this->transform.position - distance * normal;

		Collision nCollision(pos, normal, relVelocity, this->radius - distance, 1.0f, this->physicsBody);
		collisions.push(nCollision);
	}
	//*/

	///*
	//This is known, Can skip... probabl
	if (true || OctTree::boundingBox(shared_from_this(), triangle))
	{
		glm::vec3 relVelocity = this->velocity;
		std::pair<bool, glm::vec3> position = OctTree::getClosestPointInTriangle(shared_from_this(), triangle);
		if (position.first)
		{
			glm::vec3 normal = this->transform.position - position.second;
			float distance = glm::length(normal);
			if (distance <= this->radius)
			{
				normal = glm::normalize(normal);

				float p1 = 0.0;
				float p2 = 0.0;
				
				p1 = -1.0f;

				glm::vec3 momentumCorrection = p1 * this->velocity;// +p2 * collider->velocity;

				Collision nCollision(position.second, normal, momentumCorrection, this->radius - distance, this->physicsBody);
				collisions.push(nCollision);

			//	Collision nCollision(position.second, normal, relVelocity, distance, 1.0f, this->physicsBody);
			//	collisions.push(nCollision);
			}
		}
	}
	//*/

	/*
	//This isn't the best. Need to do more specific.
	float distance = OctTree::distanceToTriangle(shared_from_this(), triangle);//self

	float rectify = (this->radius) - distance;

	glm::vec3 relVelocity = this->velocity;

	//position is the center. Normal is the direction from the other ball to this ball
	glm::vec3 normal = triangle->normal;
	glm::vec3 position = this->transform.position - distance * normal;

	//Collision nCollision(position, normal, this->physicsBody);
	Collision nCollision(position, normal, relVelocity, rectify, 1.0f, this->physicsBody);
	//nCollision.impulse = (-projVelocity + rectifyingDistance);
	collisions.push(nCollision);
	//*/
}

//============================== CollisionStructure ================================

// ===================================== OctTree =======================================

OctTree::OctTreeNode::~OctTreeNode()
{
	for (int i = 0; i < 8; i++)
	{
//		delete children[i];
	}
}

glm::vec3 OctTree::OctTreeNode::getRegionVector(int region)
{
	glm::vec3 out;
	if (region < 4)		out.x = 0.0;
	else				out.x = 1.0;
	if (region % 4 < 2) out.y = 0.0;
	else				out.y = 1.0;
	if (region % 2 < 1)	out.z = 0.0;
	else				out.z = 1.0;
	return out;
}

int OctTree::OctTreeNode::getRegion(glm::vec3 point)
{
	point -= pos;
	int region = 0;
	if (point.x < 0)
		region += 4;
	else
		region += 0;

	if (point.y < 0)
		region += 2;
	else
		region += 0;

	if (point.z < 0)
		region += 1;
	else
		region += 0;
//	return 0;
	return region;
}

void OctTree::OctTreeNode::build(int depth, int maxTriangles)
{
	//std::cout << depth << " : " << faces.size() << "\n";
	if (depth <= 0 || faces.size() <= maxTriangles)
	{
		//the end
		//this->faces = faces;
		this->leaf = true;
	} 
	else
	{
		//keep building
		int numChildren = 0;
		for (int i = 0; i < 8; i++)
		{
			std::shared_ptr<OctTreeNode> nNode = std::shared_ptr<OctTreeNode>(new OctTreeNode());
			glm::vec3 regionVector = getRegionVector(i);
			//glm::vec3 mid = glm::vec3(0.5*(bounds.low.x + bounds.high.x), 0.5*(bounds.low.y + bounds.high.y), 0.5*(bounds.low.z + bounds.high.z));
			Bounds nBounds = bounds;
			if (regionVector.x == 0.0)  nBounds.low.x = pos.x;
			else						nBounds.high.x = pos.x;
			if (regionVector.y == 0.0)  nBounds.low.y = pos.y;
			else						nBounds.high.y = pos.y;
			if (regionVector.z == 0.0)  nBounds.low.z = pos.z;
			else						nBounds.high.z = pos.z;
			nNode->pos = glm::vec3(0.5*(nBounds.low.x + nBounds.high.x), 0.5*(nBounds.low.y + nBounds.high.y), 0.5*(nBounds.low.z + nBounds.high.z));
			nNode->bounds = nBounds;
			nNode->faces = nBounds.trianglesInBounds(faces);
			nNode->leaf = false;
		//	if(depth == 0)
		//	std::cout << depth <<"\t:"<< nNode->faces.size() << "\n";
			if (nNode->faces.size() == 0)
			{
				children.push_back(nullptr);
			//	delete nNode;
			}
			else
			{
				numChildren++;
				children.push_back(nNode);
				nNode->build(depth - 1, maxTriangles);
			}
		}

	//	if (numChildren == 0)
	//	{
	//		this->leaf = true;
	//	}

	//	this->children.clear();
	//	this->faces.clear();
	}
}

OctTree::OctTree()
{
	root = nullptr;
}

OctTree::~OctTree()
{
//	delete root;
}

bool OctTree::boundingBox(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle)
{
	double maxX = fmax(triangle->p1.x, fmax(triangle->p2.x, triangle->p3.x));
	double minX = fmin(triangle->p1.x, fmin(triangle->p2.x, triangle->p3.x));
	double maxY = fmax(triangle->p1.y, fmax(triangle->p2.y, triangle->p3.y));
	double minY = fmin(triangle->p1.y, fmin(triangle->p2.y, triangle->p3.y));
	double maxZ = fmax(triangle->p1.z, fmax(triangle->p2.z, triangle->p3.z));
	double minZ = fmin(triangle->p1.z, fmin(triangle->p2.z, triangle->p3.z));
	double radius = collider->radius;
	if (maxX >= collider->transform.position.x - radius && minX <= collider->transform.position.x + radius && maxY >= collider->transform.position.y - radius && minY <= collider->transform.position.y + radius && maxZ >= collider->transform.position.z - radius && minZ <= collider->transform.position.z + radius)
		return true;
	return false;
}

double OctTree::distanceToTriangle(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle)
{
	glm::vec3 p = collider->transform.position;
	glm::vec3 ba = triangle->p2 - triangle->p1; glm::vec3 pa = p - triangle->p1;
	glm::vec3 cb = triangle->p3 - triangle->p2; glm::vec3 pb = p - triangle->p2;
	glm::vec3 ac = triangle->p1 - triangle->p3; glm::vec3 pc = p - triangle->p3;
	glm::vec3 nor = glm::cross(ba, ac);

	return sqrt(
		(glm::sign(glm::dot(glm::cross(ba, nor), pa)) +
			glm::sign(glm::dot(glm::cross(cb, nor), pb)) +
			glm::sign(glm::dot(glm::cross(ac, nor), pc)) < 2.0)
		?
		glm::min(glm::min(
			glm::length(ba*glm::clamp(dot(ba, pa) / glm::length2(ba), 0.0f, 1.0f) - pa),
			glm::length(cb*glm::clamp(dot(cb, pb) / glm::length2(cb), 0.0f, 1.0f) - pb)),
			glm::length(ac*glm::clamp(dot(ac, pc) / glm::length2(ac), 0.0f, 1.0f) - pc))
		:
		glm::dot(nor, pa)*glm::dot(nor, pa) / glm::length2(nor));
}

std::pair<bool, glm::vec3> OctTree::getClosestPointInTriangle(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle)
{
	//assume we know it collides
	std::pair<bool, glm::vec3> trianglePos = getClosestPoint(triangle, collider);
	std::pair<bool, glm::vec3> l1Pos = getClosestPoint(triangle->p1, triangle->p2, collider);
	std::pair<bool, glm::vec3> l2Pos = getClosestPoint(triangle->p2, triangle->p3, collider);
	std::pair<bool, glm::vec3> l3Pos = getClosestPoint(triangle->p3, triangle->p1, collider);
	std::pair<bool, glm::vec3> out(false, glm::vec3());
	float smallestLength = -1;
	if (trianglePos.first)
	{
		//valid collision
		float length = glm::length(trianglePos.second - collider->transform.position);
		if (smallestLength == -1 || length < smallestLength)
		{
			smallestLength = length;
			out = trianglePos;
		}
	}

	if (l1Pos.first)
	{
		//valid collision
		float length = glm::length(l1Pos.second - collider->transform.position);
		if (smallestLength == -1 || length < smallestLength)
		{
			smallestLength = length;
			out = l1Pos;
		}
	}

	if (l2Pos.first)
	{
		//valid collision
		float length = glm::length(l2Pos.second - collider->transform.position);
		if (smallestLength == -1 || length < smallestLength)
		{
			smallestLength = length;
			out = l2Pos;
		}
	}

	if (l3Pos.first)
	{
		//valid collision
		float length = glm::length(l3Pos.second - collider->transform.position);
		if (smallestLength == -1 || length < smallestLength)
		{
			smallestLength = length;
			out = l3Pos;
		}
	}

	return out;
}

std::pair<bool, glm::vec3> OctTree::getClosestPoint(glm::vec3 A, glm::vec3 B, std::shared_ptr<Collider> collider)
{
	//assume we know it collides
	glm::vec3 D = collider->transform.position;
	glm::vec3 AB = B - A;
	glm::vec3 AD = D - A;
	double t = glm::dot(AD, AB) / glm::dot(AB, AB);
	if (t >= 0.0 && t <= 1.0)
	{
		glm::vec3 P = A + (float)t * AB;
		return std::pair<bool, glm::vec3>(true, P);
	}
	return std::pair<bool, glm::vec3>(false, glm::vec3());
}

std::pair<bool, glm::vec3> OctTree::getClosestPoint(std::shared_ptr<Triangle> triangle, std::shared_ptr<Collider> collider)
{
	//assume we know it collides
	double t = 0.0;
	double s = 0.0;
	glm::vec3 A = triangle->p1;
	glm::vec3 B = triangle->p2;
	glm::vec3 C = triangle->p3;
	glm::vec3 D = collider->transform.position;

	glm::vec3 AB = B - A;
	glm::vec3 AC = C - A;
	glm::vec3 AD = D - A;

	double a = glm::length2(AB);
	double b = glm::dot(AC, AB);
	double c = glm::dot(AD, AB);

	double d = glm::dot(AB, AC);
	double e = glm::length2(AC);
	double f = glm::dot(AD, AC);

	solveSystem(a, b, c, d, e, f, &t, &s);
	if (t >= 0.0 && s >= 0.0 && t + s <= 1.0)
	{
		//check distance now
		glm::vec3 P = A + (float)t * AB + (float)s * AC;
		return std::pair<bool, glm::vec3>(true, P);
	}
	return std::pair<bool, glm::vec3>(false, glm::vec3());
}

double OctTree::distanceToPlane(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle)
{
	double d = (triangle->normal.x * triangle->pos.x + triangle->normal.y * triangle->pos.y + triangle->normal.z * triangle->pos.z);
	return (triangle->normal.x * collider->transform.position.x + triangle->normal.y * collider->transform.position.y + triangle->normal.z * collider->transform.position.z - d);
}

void OctTree::solveSystem(double a, double b, double c, double d, double e, double f, double *x, double *y)
{
	*x = (c - ((b * (f - ((d*c) / a))) / (e - ((d*b) / a)))) / a;
	*y = (f - ((d*c) / a)) / (e - ((d*b) / a));
}

bool OctTree::pointInTriangle(float x, float y, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y)
{
	double t = 0.0;
	double s = 0.0;
	solveSystem(p1x - p0x, p2x - p0x, x - p0x, p1y - p0y, p2y - p0y, y - p0y, &t, &s);
	if (t >= 0.0 && s >= 0.0 && t + s <= 1.0) return true;
	return false;
}

bool OctTree::pointInCircle(float x, float y, float xc, float yc, float radius)
{
	double d = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));
	return d <= radius;
}

bool OctTree::closestPointInCirle(float Ax, float Ay, float Bx, float By, float xc, float yc, float radius)
{
	glm::vec2 AC = glm::vec2(xc - Ax, yc - Ay);
	glm::vec2 AB = glm::vec2(Bx - Ax, By - Ay);

	float magnitudeAB = glm::length2(AB);// .LengthSquared();     //Magnitude of AB vector (it's length squared)     
	float ABAPproduct = glm::dot(AC, AB);// Vector2.Dot(AP, AB);    //The DOT product of a_to_p and a_to_b     
	float distance = ABAPproduct / magnitudeAB; //The normalized "distance" from a to your closest point  

	if (distance < 0.0 || distance > 1.0)     //Check if P projection is over vectorAB     
	{
		return false;
	}
	else
	{
		glm::vec2 P = glm::vec2(Ax, Ay) + distance * AB;
		return glm::length(glm::vec2(xc, yc) - P) <= radius;
	}
}

bool OctTree::projectionCollisions(float collX, float collY, float radius, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y)
{
	if (pointInCircle(p1x, p1y, collX, collY, radius)) return true;
	if (pointInCircle(p2x, p2y, collX, collY, radius)) return true;
	if (pointInCircle(p3x, p3y, collX, collY, radius)) return true;
	if (pointInTriangle(collX, collY, p1x, p1y, p2x, p2y, p3x, p3y)) return true;
	if (closestPointInCirle(p1x, p1y, p2x, p2y, collX, collY, radius)) return true;
	if (closestPointInCirle(p2x, p2y, p3x, p3y, collX, collY, radius)) return true;
	if (closestPointInCirle(p3x, p3y, p1x, p1y, collX, collY, radius)) return true;
	return false;
}

bool OctTree::pointInSphere(glm::vec3 point, std::shared_ptr<Collider> collider)
{
	double d = glm::length(point - collider->transform.position);// sqrt((point.x - collider->transform.position.x) * (point.x - collider->transform.position.x) + (point.y - collider->transform.position.y) * (point.y - collider->transform.position.y) + (point.z - collider->transform.position.z) * (point.z - collider->transform.position.z));
	return d <= collider->radius;
}

bool OctTree::closestPointInSphere(glm::vec3 A, glm::vec3 B, std::shared_ptr<Collider> collider)
{
	glm::vec3 D = collider->transform.position;
	glm::vec3 AB = B - A;
	glm::vec3 AD = D - A;
	double t = glm::dot(AD, AB) / glm::dot(AB, AB);
	if (t >= 0.0 && t <= 1.0)
	{
		glm::vec3 P = A + (float) t * AB;
		return glm::length(P - D) <= collider->radius;
	}
	return false;
}

bool OctTree::closestPointInSphere(std::shared_ptr<Triangle> triangle, std::shared_ptr<Collider> collider)
{
	double t = 0.0;
	double s = 0.0;
	glm::vec3 A = triangle->p1;
	glm::vec3 B = triangle->p2;
	glm::vec3 C = triangle->p3;
	glm::vec3 D = collider->transform.position;

	glm::vec3 AB = B - A;
	glm::vec3 AC = C - A;
	glm::vec3 AD = D - A;
	
	double a = glm::length2(AB);
	double b = glm::dot(AC, AB);
	double c = glm::dot(AD, AB);

	double d = glm::dot(AB, AC);
	double e = glm::length2(AC);
	double f = glm::dot(AD, AC);

	solveSystem(a, b, c, d, e, f, &t, &s);
	if (t >= 0.0 && s >= 0.0 && t + s <= 1.0)
	{
		//check distance now
		glm::vec3 P = A + (float) t * AB + (float) s * AC;
		return glm::length(P - D) <= collider->radius;
	}
	return false;
}

bool OctTree::collideFace(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle)
{
	if (boundingBox(collider, triangle))
	{
		//Better and pretty
		if (distanceToTriangle(collider, triangle) <= collider->radius)
			return true;
	//	std::cout << distanceToTriangle(collider, triangle) << " \t: " << collider->radius << "\n";
		//OLD AND SHITTY. Miss me with it's complicated stuff
		///*
		//if (fabs(distanceToPlane(collider, triangle)) <= collider->radius)
		//{
		//	if (closestPointInSphere(triangle, collider)) return true;
		//	if (closestPointInSphere(triangle->p1, triangle->p2, collider)) return true;
		//	if (closestPointInSphere(triangle->p2, triangle->p3, collider)) return true;
		//	if (closestPointInSphere(triangle->p3, triangle->p1, collider)) return true;
		//	return false;
		//}
		//*/
	}
	return false;
}

void OctTree::build(std::shared_ptr<Mesh> mesh, int depth, int maxTriangle)
{
	std::vector<std::shared_ptr<Triangle>> faces = mesh->toTriangles();
	root = std::shared_ptr<OctTreeNode>(new OctTreeNode());
	root->faces = faces;
	root->leaf = false;
	root->bounds = Bounds::boundsFromTriangles(faces);
	root->pos = glm::vec3(0.5*(root->bounds.low.x + root->bounds.high.x), 0.5*(root->bounds.low.y + root->bounds.high.y), 0.5*(root->bounds.low.z + root->bounds.high.z));
	root->build(depth, maxTriangle);
}

void OctTree::addObjectMesh(std::shared_ptr<GameObject> gameObject, std::vector<std::shared_ptr<Triangle>> & faces)
{
	if (gameObject->mesh != nullptr && gameObject->collidable)
	{
		std::vector<std::shared_ptr<Triangle>> toAdd = gameObject->mesh->toTriangles(gameObject->transform.getTransform());
		for (int i = 0; i < toAdd.size(); i++)
		{
			faces.push_back(toAdd[i]);
		}
	}
	for (int i = 0; i < gameObject->children.size(); i++)
	{
		addObjectMesh(gameObject->children[i], faces);
	}
}

void OctTree::build(std::shared_ptr<GameObject> gameObject, int depth, int maxTriangle)
{
	std::vector<std::shared_ptr<Triangle>> faces;
	addObjectMesh(gameObject, faces);
	root = std::shared_ptr<OctTreeNode>(new OctTreeNode());
	root->faces = faces;
	root->bounds = Bounds::boundsFromTriangles(faces);
	float max = fmax(root->bounds.high.x, fmax(root->bounds.high.y, root->bounds.high.z));
	float min = fmax(root->bounds.low.x, fmax(root->bounds.low.y, root->bounds.low.z));
//	root->bounds.high = glm::vec3(max);
//	root->bounds.low = glm::vec3(min);
	root->pos = glm::vec3(0.5*(root->bounds.low.x + root->bounds.high.x), 0.5*(root->bounds.low.y + root->bounds.high.y), 0.5*(root->bounds.low.z + root->bounds.high.z));
	root->build(depth, maxTriangle);
}

std::shared_ptr<ColorMesh> OctTree::combineMeshes(std::shared_ptr<GameObject> gameObject, int r, int g, int b)
{
	std::vector<std::shared_ptr<Triangle>> faces;
	addObjectMesh(gameObject, faces);
	return ColorMesh::meshFromTriangles(faces, r, g, b);
}

std::vector<std::shared_ptr<Triangle>> OctTree::collide_old(std::shared_ptr<Collider> collider)
{
	std::shared_ptr<OctTreeNode> ptr = root;
	std::vector<std::shared_ptr<Triangle>> triangles;
	while (ptr != nullptr && ptr->leaf == false)
	{
		glm::vec3 point = collider->transform.position;
		ptr = ptr->children[ptr->getRegion(point)];
	}
	
	if (ptr != nullptr)
	{
		for (int i = 0; i < ptr->faces.size(); i++)
		{
			if (collideFace(collider, ptr->faces[i]))
			{
				triangles.push_back(ptr->faces[i]);
			}
		}
	}
	return triangles;
}

std::vector<std::shared_ptr<Triangle>> OctTree::collide(std::shared_ptr<Collider> collider)
{
	return nodeCollide(root, collider);
}

std::vector<std::shared_ptr<Triangle>> OctTree::nodeCollide(std::shared_ptr<OctTreeNode> node, std::shared_ptr<Collider> collider)
{
	std::vector<std::shared_ptr<Triangle>> triangles;
	if (node->bounds.sphereBBInBounds(collider->transform.position, collider->radius))
	{
		if (node->leaf)
		{
			for (int i = 0; i < node->faces.size(); i++)
			{
				if (collideFace(collider, node->faces[i]))
				{
					triangles.push_back(node->faces[i]);
				}
			}
		}
		else
		{
			//keep on sliding...
			for (int i = 0; i < 8; i++)
			{
				if (node->children[i])
				{
					std::vector<std::shared_ptr<Triangle>> region = nodeCollide(node->children[i], collider);
					triangles.insert(std::end(triangles), std::begin(region), std::end(region));
				}
			}
		}
	}
	return triangles;
}

// ========================== Grid ==================================

//=========================== ColliderGrid ==========================

ColliderGrid::ColliderGrid(Bounds bounds, glm::vec3 numCells)
{
	this->bounds = bounds;
	this->numCells = glm::floor(numCells);
	cellSize = (bounds.high - bounds.low) / numCells;
	setGrid(this->numCells);
}

ColliderGrid::~ColliderGrid()
{
}

std::vector<glm::vec3> ColliderGrid::getCellList(std::shared_ptr<Collider> collider)
{
	std::vector<glm::vec3> out;
	
	glm::vec3 gridPos = glm::floor(collider->transform.position / cellSize);
	glm::vec3 gridRadius = glm::ceil(glm::vec3(collider->radius) / cellSize);

	for (int x = gridPos.x - gridRadius.x; x <= gridPos.x + gridRadius.x; x++)
	{
		for (int y = gridPos.y - gridRadius.y; y <= gridPos.y + gridRadius.y; y++)
		{
			for (int z = gridPos.z - gridRadius.z; z <= gridPos.z + gridRadius.z; z++)
			{
				if (x >= 0 && x < numCells.x && y >= 0 && y < numCells.y && z >= 0 && z < numCells.z)
				{
					//test if box is in? Or just add and pray....
					float d = Geometry::distanceFromBox(collider->transform.position, cellSize * glm::vec3(x + 0.5, y + 0.5, z + 0.5), 0.5f * cellSize);
					if (d <= collider->radius)
						out.push_back(glm::vec3(x, y, z));
				}
			}
		}
	}

	/*
	for (float x = pos.x - radius; x <= pos.x + radius; x += cellSize.x)
	{
		for (float y = pos.y - radius; y <= pos.y + radius; y += cellSize.y)
		{
			for (float z = pos.z - radius; z <= pos.z + radius; x += cellSize.z)
			{
				glm::vec3 cPos = cellSize * glm::floor((pos - glm::vec3(radius) + glm::vec3(x, y, z)) / cellSize);
				if(cPos.x >= 0 && cPos.y >= 0 && cPos.z >= 0 && cPos.x < numCells.x && cPos.y < numCells.y && cPos.z < numCells.z)
					out.push_back(cPos);
			}
		}
	}
	*/
	return out;
}

void ColliderGrid::setGrid(glm::vec3 numCells)
{
	grid = std::vector < std::vector<std::vector<std::vector<std::shared_ptr<Collider>>>>>(numCells.x);
	for (int i = 0; i < numCells.x; i++)
	{
		grid[i] = std::vector < std::vector<std::vector<std::shared_ptr<Collider>>>>(numCells.y);
		for (int j = 0; j < numCells.y; j++)
		{
			grid[i][j] = std::vector<std::vector<std::shared_ptr<Collider>>>(numCells.z);
			for (int k = 0; k < numCells.z; k++)
			{
				grid[i][j][k] = std::vector<std::shared_ptr<Collider>>();
			}
		}
	}
}

void ColliderGrid::resetGrid()
{
	for (int i = 0; i < numCells.x; i++)
	{
		for (int j = 0; j < numCells.y; j++)
		{
			for (int k = 0; k < numCells.z; k++)
			{
				grid[i][j][k].clear();
			}
		}
	}
}

void ColliderGrid::collideCell(glm::vec3 cell, std::shared_ptr<Collider> collider, std::stack<Collision>& collisions)
{
	for (int i = 0; i < grid[cell.x][cell.y][cell.z].size(); i++)
	{
		collider->collision(grid[cell.x][cell.y][cell.z][i], collisions);
	}
}

bool ColliderGrid::addColliderToGrid(std::shared_ptr<Collider> collider)
{
	std::vector<glm::vec3> cellList = getCellList(collider);
	for (int i = 0; i < cellList.size(); i++)
	{
		grid[cellList[i].x][cellList[i].y][cellList[i].z].push_back(collider);
	}
	if (cellList.size() > 0)
	{
		this->colliderList.push_back(collider);
	}
	return cellList.size() > 0;
}

std::vector<std::shared_ptr<Collider>> ColliderGrid::update()
{
	resetGrid();
	std::vector<std::shared_ptr<Collider>> removed;
	for (int i = 0; i < colliderList.size(); i++)
	{
		if (!this->addColliderToGrid(colliderList[i]))
		{
			removed.push_back(colliderList[i]);
			colliderList.erase(colliderList.begin() + i);
			i--;
		}
	}
	return removed;
}

void ColliderGrid::collision(std::shared_ptr<Collider> collider, std::stack<Collision>& collisions)
{
	std::vector<glm::vec3> cells = getCellList(collider);
	for (int i = 0; i < cells.size(); i++)
	{
		collideCell(cells[i], collider, collisions);
	}
}

//=========================== ColliderGrid ==========================

infiniteColliderGrid::infiniteColliderGrid(glm::vec3 cellSize)
{
	this->cellSize = cellSize;
}

infiniteColliderGrid::~infiniteColliderGrid()
{
}

std::vector<std::tuple<int, int, int>> infiniteColliderGrid::getCellList(std::shared_ptr<Collider> collider)
{
	std::vector<std::tuple<int, int, int>> out;

	glm::vec3 gridPos = glm::floor(collider->transform.position / cellSize);
	glm::vec3 gridRadius = glm::ceil(glm::vec3(collider->radius) / cellSize);

	for (int x = gridPos.x - gridRadius.x; x <= gridPos.x + gridRadius.x; x++)
	{
		for (int y = gridPos.y - gridRadius.y; y <= gridPos.y + gridRadius.y; y++)
		{
			for (int z = gridPos.z - gridRadius.z; z <= gridPos.z + gridRadius.z; z++)
			{
				float d = Geometry::distanceFromBox(collider->transform.position, cellSize * glm::vec3(x + 0.5, y + 0.5, z + 0.5), 0.5f * cellSize);
				if (d <= collider->radius)
					out.push_back(std::tuple<int, int, int>(x, y, z));
			}
		}
	}

	return out;
}

void infiniteColliderGrid::setGrid(glm::vec3 numCells)
{

}

void infiniteColliderGrid::resetGrid()
{

}

std::vector<std::shared_ptr<Collider>> infiniteColliderGrid::collideCell(std::tuple<int, int, int> cell, std::shared_ptr<Collider> collider, std::stack<Collision>& collisions)
{
	std::vector<std::shared_ptr<Collider>> out;
	if (grid.find(cell) != grid.end())
	{
		int val = collisions.size();
		for (int i = 0; i < grid[cell].size(); i++)
		{
			collider->collision(grid[cell][i], collisions);
			int nVal = collisions.size();
			if (nVal != val)
			{
				//did collide. Push back
				val = nVal;
				out.push_back(grid[cell][i]);
			}
		}
	}
	return out;
}

bool infiniteColliderGrid::addColliderAtIndex(std::tuple<int, int, int> cell, std::shared_ptr<Collider> collider)
{
	if (grid.find(cell) != grid.end())
	{
		grid[cell].push_back(collider);
	}
	else
	{
		grid[cell] = std::vector<std::shared_ptr<Collider>>();
		grid[cell].push_back(collider);
	}
	return true;
}

bool infiniteColliderGrid::pushColliderToGrid(std::shared_ptr<Collider> collider)
{
	std::vector<std::tuple<int, int, int>> cellList = getCellList(collider);
	for (int i = 0; i < cellList.size(); i++)
	{
		addColliderAtIndex(cellList[i], collider);
	}
	return true;
}

std::vector<std::shared_ptr<Collider>> infiniteColliderGrid::getCollidersInCell(std::tuple<int, int, int> cell)
{
	std::vector<std::shared_ptr<Collider>> out;
	if (grid.find(cell) != grid.end())
	{
		out = grid[cell];
	}
	return out;
}

bool infiniteColliderGrid::addColliderToGrid(std::shared_ptr<Collider> collider)
{
	colliderList.push_back(collider);
	return pushColliderToGrid(collider);
}

std::vector<std::shared_ptr<Collider>> infiniteColliderGrid::update()
{
/*	for (auto & p = grid.cbegin(); p != grid.cend(); p++)
	{
		
	}*/
	grid.clear();

	for (int i = 0; i < colliderList.size(); i++)
	{
		this->pushColliderToGrid(colliderList[i]);
	}

	/*
	for (auto p = grid.cbegin(); p != grid.cend();)
	{
		if()
		{
			grid.erase(p++);
		}
		else
		{
			p++
		}
	}*/

	return std::vector<std::shared_ptr<Collider>>();
}

void infiniteColliderGrid::collision(std::shared_ptr<Collider> collider, std::stack<Collision>& collisions)
{
	std::vector<std::tuple<int, int, int>> cells = getCellList(collider);
	std::vector<std::shared_ptr<Collider>> didCollide;
	for (int i = 0; i < cells.size(); i++)
	{
		std::vector<std::shared_ptr<Collider>> result = getCollidersInCell(cells[i]);
		didCollide.insert(std::end(didCollide), std::begin(result), std::end(result));
	}
	//go through and filter out all duplicates...
	std::unordered_set<std::shared_ptr<Collider>> s;
	for (std::shared_ptr<Collider> i : didCollide)
		s.insert(i);
	didCollide.assign(s.begin(), s.end());
	std::sort(didCollide.begin(), didCollide.end());

	for (int i = 0; i < didCollide.size(); i++)
	{
		collider->collision(didCollide[i], collisions);
	}
}

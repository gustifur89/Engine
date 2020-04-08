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

	if (distance <= this->radius + collider->radius)
	{
		float lockedCorrection = (this->locked | collider->locked) ? 1.0f : 0.5f;

		float rectify = (this->radius + collider->radius) - distance;

		glm::vec3 relVelocity = this->velocity - collider->velocity;

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
		Collision nCollision(position, normal, relVelocity, rectify, lockedCorrection, this->physicsBody);
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

	//Now we need to do grahm-schmidt on the results to avoid double counting and stuff. It caused errors where balls would just bounce off of meshes.




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
	float distance = OctTree::distanceToTriangle(shared_from_this(), triangle);//self

	float rectify = (this->radius) - distance;

	glm::vec3 relVelocity = this->velocity;

	//position is the center. Normal is the direction from the other ball to this ball
	glm::vec3 normal = triangle->normal;
	glm::vec3 position = this->transform.position - distance * normal;

	/*
	float dot = glm::dot(normal, relVelocity);
	glm::vec3 projVelocity = dot < 0 ? dot * normal : glm::vec3(0);

	//impulse should cancel the velocity into the object and offset back out of object
	glm::vec3 rectifyingDistance = rectify * normal;

	//	this->physicsBody->applyImpulse(bothOffset * (-projVelocity + rectifyingDistance), glm::vec3(0, 0, 0));
	*/

	//Collision nCollision(position, normal, this->physicsBody);
	Collision nCollision(position, normal, relVelocity, rectify, 1.0f, this->physicsBody);
	//nCollision.impulse = (-projVelocity + rectifyingDistance);
	collisions.push(nCollision);
}

//============================== CollisionStructure ================================

CollisionStructure::CollisionStructure()
{
}

CollisionStructure::~CollisionStructure()
{
}

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
	root->pos = glm::vec3(0.5*(root->bounds.low.x + root->bounds.high.x), 0.5*(root->bounds.low.y + root->bounds.high.y), 0.5*(root->bounds.low.z + root->bounds.high.z));
	root->build(depth, maxTriangle);
}

std::shared_ptr<ColorMesh> OctTree::combineMeshes(std::shared_ptr<GameObject> gameObject, int r, int g, int b)
{
	std::vector<std::shared_ptr<Triangle>> faces;
	addObjectMesh(gameObject, faces);
	return ColorMesh::meshFromTriangles(faces, r, g, b);
}

std::vector<std::shared_ptr<Triangle>> OctTree::collide(std::shared_ptr<Collider> collider)
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

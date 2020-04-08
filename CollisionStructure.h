#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "Geometry.h"
#include "CollisionDeclaration.h"
#include "PhysicsBody.h"

struct Collision
{
public:
	Collision(glm::vec3 position, glm::vec3 normal, glm::vec3 relVelocity, float rectifyingDistance, float lockedCorrection, std::shared_ptr<PhysicsBody> physicsBody)
	{
		this->position = position;
		this->normal = normal;
		this->physicsBody = physicsBody;
		this->lockedCorrection = lockedCorrection;
		this->relVelocity = relVelocity;
		this->rectifyingDistance = rectifyingDistance;
	}

	float lockedCorrection; //if both objects collide or if it's only one body
	float rectifyingDistance; //distance to stop collisions
	glm::vec3 relVelocity;//relative velocity of this bpody in relation to the other.
	glm::vec3 normal; //normal of the collision
	glm::vec3 position; //location the collision occurs

	//glm::vec3 impulse;
	//glm::vec3 position;
	//glm::vec3 normal;
	std::shared_ptr<PhysicsBody> physicsBody;
};

class Collider : public std::enable_shared_from_this<Collider>
{
public:
	Collider();
	Collider(float radius);
	~Collider();
	Transform transform;
	glm::vec4 relPos;
	glm::vec3 velocity;
	void update(glm::mat4 & localToWorld, glm::vec3 & velocity, bool locked);

	//	glm::vec3 * vel;
//	glm::vec3 * position;
	float radius;  
	bool locked;
	int colliderType;
//	std::shared_ptr<Collider> self;
	std::shared_ptr<PhysicsBody> physicsBody;
	//methods:
	void collision(std::shared_ptr<Collider> collider, std::stack<Collision> & collisions);
	void collision(std::shared_ptr<Collider> collider);
	void collision(std::shared_ptr<CollisionStructure> collisionStructure, std::stack<Collision> & collisions);
	void collision(std::shared_ptr<Triangle> triangle, std::stack<Collision> & collisions);
};

class CollisionStructure
{
public:
	CollisionStructure();
	~CollisionStructure();


	virtual std::vector<std::shared_ptr<Triangle>> collide(std::shared_ptr<Collider> collider) = 0;
};

class OctTree : public CollisionStructure
{
private:
	class OctTreeNode
	{
	public:
		Bounds bounds;
		glm::vec3 pos;
		std::vector<std::shared_ptr<Triangle>> faces;
		std::vector<std::shared_ptr<OctTreeNode>> children;
		bool leaf;
		~OctTreeNode();
		void build(int depth, int maxTriangles);//std::vector<Triangle> faces,
		int getRegion(glm::vec3 point);
		glm::vec3 getRegionVector(int region);
	};

	//void build(OctTreeNode * node, std::vector<Triangle> faces, int depth, int maxTriangles);

	static void addObjectMesh(std::shared_ptr<GameObject> gameObject, std::vector<std::shared_ptr<Triangle>> & faces);

public:
	std::shared_ptr<OctTreeNode> root;
	void build(std::shared_ptr<Mesh> mesh, int depth, int maxTriangle);
	void build(std::shared_ptr<GameObject> gameObject, int depth, int maxTriangle);
	std::vector<std::shared_ptr<Triangle>> collide(std::shared_ptr<Collider> collider);
	bool projectionCollisions(float collX, float collY, float radius, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);
	void solveSystem(double a, double b, double c, double d, double e, double f, double *x, double *y);
	bool pointInTriangle(float x, float y, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y);
	bool pointInCircle(float x, float y, float xc, float yc, float radius);
	bool pointInSphere(glm::vec3 point, std::shared_ptr<Collider> collider);
	bool closestPointInCirle(float Ax, float Ay, float Bx, float By, float xc, float yc, float radius);
	bool closestPointInSphere(glm::vec3 A, glm::vec3 B, std::shared_ptr<Collider> collider);
	bool closestPointInSphere(std::shared_ptr<Triangle> triangle, std::shared_ptr<Collider> collider);
	double distanceToPlane(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	bool collideFace(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	bool boundingBox(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	static double distanceToTriangle(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);

	static std::shared_ptr<ColorMesh> combineMeshes(std::shared_ptr<GameObject> gameObject, int r, int g, int b);

	OctTree();
	~OctTree();
};

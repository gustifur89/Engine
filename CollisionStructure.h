#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "Geometry.h"
#include "CollisionDeclaration.h"
#include "PhysicsBody.h"

struct Collision
{
public:
	/*
	Collision(glm::vec3 position, glm::vec3 normal, glm::vec3 relVelocity, float rectifyingDistance, float momentumCorrection, std::shared_ptr<PhysicsBody> physicsBody)
	{
		this->position = position;
		this->normal = normal;
		this->physicsBody = physicsBody;
		this->momentumCorrection = momentumCorrection;
		this->relVelocity = relVelocity;
		this->rectifyingDistance = rectifyingDistance;
	}
	float momentumCorrection; //if both objects collide or if it's only one body
	float rectifyingDistance; //distance to stop collisions
	glm::vec3 reactionImpulse;
	glm::vec3 relVelocity;//relative velocity of this bpody in relation to the other.
	glm::vec3 normal; //normal of the collision
	glm::vec3 position; //location the collision occurs
	*/
	Collision(glm::vec3 position, glm::vec3 normal, glm::vec3 reactionImpulse, float rectifyingDistance, std::shared_ptr<PhysicsBody> physicsBody)
	{
		this->position = position;
		this->normal = normal;
		this->reactionImpulse = reactionImpulse;
		this->physicsBody = physicsBody;
		this->rectifyingDistance = rectifyingDistance;
	}

	glm::vec3 reactionImpulse;	//relative velocity of this bpody in relation to the other.
	glm::vec3 normal;			//normal of the collision
	glm::vec3 position;			//location the collision occurs
	float rectifyingDistance; //distance to stop collisions

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
	std::vector<std::shared_ptr<Triangle>> collide_old(std::shared_ptr<Collider> collider);
	std::vector<std::shared_ptr<Triangle>> collide(std::shared_ptr<Collider> collider);
	std::vector<std::shared_ptr<Triangle>> nodeCollide(std::shared_ptr<OctTreeNode> node, std::shared_ptr<Collider> collider);
	bool projectionCollisions(float collX, float collY, float radius, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);
	static void solveSystem(double a, double b, double c, double d, double e, double f, double *x, double *y);
	static bool pointInTriangle(float x, float y, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y);
	static bool pointInCircle(float x, float y, float xc, float yc, float radius);
	static bool pointInSphere(glm::vec3 point, std::shared_ptr<Collider> collider);
	static bool closestPointInCirle(float Ax, float Ay, float Bx, float By, float xc, float yc, float radius);
	static bool closestPointInSphere(glm::vec3 A, glm::vec3 B, std::shared_ptr<Collider> collider);
	static bool closestPointInSphere(std::shared_ptr<Triangle> triangle, std::shared_ptr<Collider> collider);
	static double distanceToPlane(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	static bool collideFace(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	static bool boundingBox(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	static double distanceToTriangle(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	static std::pair<bool, glm::vec3> getClosestPointInTriangle(std::shared_ptr<Collider> collider, std::shared_ptr<Triangle> triangle);
	static std::pair<bool, glm::vec3> getClosestPoint(glm::vec3 A, glm::vec3 B, std::shared_ptr<Collider> collider);
	static std::pair<bool, glm::vec3> getClosestPoint(std::shared_ptr<Triangle> triangle, std::shared_ptr<Collider> collider);
	static std::shared_ptr<ColorMesh> combineMeshes(std::shared_ptr<GameObject> gameObject, int r, int g, int b);

	OctTree();
	~OctTree();
};

class Grid
{
public:
	virtual bool addColliderToGrid(std::shared_ptr<Collider> collider) = 0;
	virtual void collision(std::shared_ptr<Collider> collider, std::stack<Collision>& collisions) = 0;
	virtual std::vector<std::shared_ptr<Collider>> update() = 0;
};

class ColliderGrid : public Grid
{
private:
	std::vector<glm::vec3> getCellList(std::shared_ptr<Collider> collider);
	void setGrid(glm::vec3 numCells);
	void resetGrid();
	void collideCell(glm::vec3 cell, std::shared_ptr<Collider> collider, std::stack<Collision>& collisions);

public:
	glm::vec3 numCells;
	glm::vec3 cellSize;
	Bounds bounds;
	std::vector<std::vector<std::vector<std::vector<std::shared_ptr<Collider>>>>> grid;
	std::vector<std::shared_ptr<Collider>> colliderList;

	ColliderGrid(Bounds bounds, glm::vec3 numCells);
	~ColliderGrid();



	bool addColliderToGrid(std::shared_ptr<Collider> collider);
	std::vector<std::shared_ptr<Collider>> update();
	void collision(std::shared_ptr<Collider> collider, std::stack<Collision>& collisions);
};

class infiniteColliderGrid : public Grid
{
private:
	std::vector<std::tuple<int, int, int>> getCellList(std::shared_ptr<Collider> collider);
	void setGrid(glm::vec3 numCells);
	void resetGrid();
	std::vector<std::shared_ptr<Collider>> collideCell(std::tuple<int, int, int>, std::shared_ptr<Collider> collider, std::stack<Collision>& collisions);
	bool addColliderAtIndex(std::tuple<int, int, int>, std::shared_ptr<Collider> collider);
	bool pushColliderToGrid(std::shared_ptr<Collider> collider);
	std::vector<std::shared_ptr<Collider>> getCollidersInCell(std::tuple<int, int, int>);
public:
	glm::vec3 cellSize;
	std::map<std::tuple<int,int,int>, std::vector<std::shared_ptr<Collider>>> grid;
	std::vector<std::shared_ptr<Collider>> colliderList;

	infiniteColliderGrid(glm::vec3 cellSize);
	~infiniteColliderGrid();	

	bool addColliderToGrid(std::shared_ptr<Collider> collider);
	std::vector<std::shared_ptr<Collider>> update();
	void collision(std::shared_ptr<Collider> collider, std::stack<Collision>& collisions);
};


class CollisionHandler
{
public:
	std::vector<std::shared_ptr<CollisionStructure>> collisionStructures;
	std::vector<std::shared_ptr<Collider>> colliders;

	CollisionHandler();
	~CollisionHandler();

	bool addCollider(std::shared_ptr<Collider> collider);
	bool addCollisionStructure(std::shared_ptr<CollisionStructure> collisionStructure);

	void collisionIteration();
};

#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "Geometry.h"

class Collider
{
public:
	glm::vec3 * vel;
	glm::vec3 * position;
	float radius;
};


class CollisionStructure
{
public:
	CollisionStructure();
	~CollisionStructure();


	virtual std::vector<std::shared_ptr<Triangle>> collide(Collider * collider) = 0;
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
	std::vector<std::shared_ptr<Triangle>> collide(Collider * collider);
	bool projectionCollisions(float collX, float collY, float radius, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y);
	void solveSystem(double a, double b, double c, double d, double e, double f, double *x, double *y);
	bool pointInTriangle(float x, float y, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y);
	bool pointInCircle(float x, float y, float xc, float yc, float radius);
	bool pointInSphere(glm::vec3 point, Collider * collider);
	bool closestPointInCirle(float Ax, float Ay, float Bx, float By, float xc, float yc, float radius);
	bool closestPointInSphere(glm::vec3 A, glm::vec3 B, Collider * collider);
	bool closestPointInSphere(std::shared_ptr<Triangle> triangle, Collider * collider);
	double distanceToPlane(Collider * collider, std::shared_ptr<Triangle> triangle);
	bool collideFace(Collider * collider, std::shared_ptr<Triangle> triangle);
	bool boundingBox(Collider * collider, std::shared_ptr<Triangle> triangle);
	double distanceToTriangle(Collider * collider, std::shared_ptr<Triangle> triangle);

	static std::shared_ptr<ColorMesh> combineMeshes(std::shared_ptr<GameObject> gameObject, int r, int g, int b);

	OctTree();
	~OctTree();
};

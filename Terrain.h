#pragma once
#include "Headers.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Noise.h"
#include "Entity.h"
#include "CollisionStructure.h"

class Chunk : public GameObjectColor
{
public:
	//Variables
	std::pair<double, double> key;
	bool toDelete;
	bool toAdd;
	std::shared_ptr<OctTree> octTree;

	//Methods
	Chunk();
	~Chunk();
};

class Terrain : public GameObjectColor, public CollisionStructure
{
private:
	int maxTreesPerChunk;
	double heightFunction(double y);
	void chunkManagementLoop();


public:
	//variables
	bool chunkLoop;
	glm::vec3 chunkSize;
	double meshDensity;
	glm::vec3 scale;
	int groundHeight;
	double normalizedGroundHeight;
	double treeDensity;
	int chunkDistance;
	double meshScale;
	bool genTrees;
	siv::PerlinNoise noise;
	std::shared_ptr<ColorMesh> treeMesh;
	std::shared_ptr<Entity> chunkTarget;
	std::map<std::pair<double,double>, std::shared_ptr<Chunk>> chunkMap;
	bool flatTop;

	//methods

	void addNew(glm::vec2 pos);
	void cullOld(glm::vec2 center);

	Terrain(std::uint32_t seed, glm::vec3 chunkSize, int groundHeight, double meshDensity, double meshScale, glm::vec3 scale, double treeDensity, int chunkDistance);
	~Terrain();
	void makeChunk(double x, double z);
	std::shared_ptr<Chunk> generateChunk(glm::vec2 pos, std::pair<double, double> point);
	void update();
	void renderFunc(Camera& camera, glm::mat4 parentTransform);
	void setTarget(std::shared_ptr<Entity> ent);
	bool ground(double x, double y, double z);
	double ground(double x, double z);
	void closeThread();
	void chunkManagement();
	std::vector<std::shared_ptr<Triangle>> collide(std::shared_ptr<Collider> collider);

};



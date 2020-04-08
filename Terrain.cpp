#include "Terrain.h"

// =============================== Terrain ===============================

Terrain::Terrain(std::uint32_t seed, glm::vec3 chunkSize, int groundHeight, double meshDensity, double meshScale, glm::vec3 scale, double treeDensity, int chunkDistance) : GameObjectColor()
{
	this->chunkSize = chunkSize;
	this->meshDensity = meshDensity;
	this->scale = scale;
	this->meshScale = meshScale;
	this->groundHeight = groundHeight;
	this->chunkDistance = chunkDistance;
	normalizedGroundHeight = (double) groundHeight / chunkSize.y;
	this->treeDensity = treeDensity;
	if (treeDensity <= 0.0) genTrees = false;
	else genTrees = true;
	maxTreesPerChunk = ceil(chunkSize.x * chunkSize.z * treeDensity);
	noise.reseed(seed);
	treeMesh = ColorMesh::loadFromFile("pillar");//tree
	chunkTarget = std::shared_ptr<Entity>(NULL);
	chunkLoop = true;
	this->persistentVisible = true;
	flatTop = false;
//	std::thread chunkManagementThread(&Terrain::chunkManagementLoop, this);
//	chunkManagementThread.detach();

}

Terrain::~Terrain()
{

}

void Terrain::makeChunk(double x, double z)
{
//	std::shared_ptr<Chunk> chunk(new Chunk);
//	chunk->transform.x = x;
//	chunk->transform.z = z;
}

std::shared_ptr<Chunk> Terrain::generateChunk(glm::vec2 pos, std::pair<double, double> point)
{
	std::shared_ptr<Chunk> chunk(new Chunk());
	chunk->persistentVisible = true;
	chunk->transform.position.x = pos.x * chunkSize.x * scale.x;
	chunk->transform.position.z = pos.y * chunkSize.z * scale.z;
	chunk->shader = this->shader;
	chunk->key = point;

	if (flatTop)
	{
		int xVerts = ceil(meshDensity * chunkSize.x);
		int yVerts = ceil(meshDensity * chunkSize.y);
		int zVerts = ceil(meshDensity * chunkSize.z);

		int lowX = pos.x;// meshDensity * chunkSize.x * pos.x;
		int lowZ = pos.y;

		std::vector<std::vector<double>> grid = std::vector<std::vector<double>>(xVerts + 1);

		for (int x = 0; x <= xVerts; x++)
		{
			grid[x] = std::vector<double>(zVerts + 1);
			for (int z = 0; z <= zVerts; z++)
			{
				//grid[x][z]
				
				double x_ = ((double)x) / xVerts;
				double z_ = ((double)z) / zVerts;

				grid[x][z] = meshScale * ground((lowX + x_) / meshScale, (lowZ + z_) / meshScale);

			}
		}
	
		Bounds bounds(glm::vec3(0, 0, 0), glm::vec3(scale.x * chunkSize.x, scale.y * chunkSize.y, scale.z * chunkSize.z));
		chunk->mesh = ColorMesh::meshFromVertexGrid(grid, bounds, 0, 0, 255);
		chunk->octTree->build(chunk, 4, 2);
	}
	else
	{
		int xVerts = ceil(meshDensity * chunkSize.x);
		int yVerts = ceil(meshDensity * chunkSize.y);
		int zVerts = ceil(meshDensity * chunkSize.z);

		int lowX = pos.x;// meshDensity * chunkSize.x * pos.x;
		int lowZ = pos.y;// meshDensity * chunkSize.x * pos.y;

		std::vector<std::vector<std::vector<bool>>> grid = std::vector<std::vector<std::vector<bool>>>(xVerts + 1);

		for (int x = 0; x <= xVerts; x++)
		{
			grid[x] = std::vector<std::vector<bool>>(yVerts + 1);
			for (int y = 0; y <= yVerts; y++)
			{
				grid[x][y] = std::vector<bool>(zVerts + 1);
				for (int z = 0; z <= zVerts; z++)
				{
					//	double x_ = ((double)x - xVerts / ) / xVerts;
					//	double z_ = ((double)z - zVerts / 2) / zVerts;
					double x_ = ((double)x) / xVerts;
					double y_ = ((double)y) / yVerts;
					double z_ = ((double)z) / zVerts;

					grid[x][y][z] = ground((lowX + x_) / meshScale, 2.0 * (y_ / meshScale), (lowZ + z_) / meshScale);

					if (y == yVerts)
						grid[x][y][z] = 0;
					if (y == 0)
						grid[x][y][z] = 1;
				}
			}
		}

		if (genTrees)
		{
			int numTrees = rand() % maxTreesPerChunk;
			for (int i = 0; i < numTrees; i++)
			{
				int x = rand() % xVerts;
				int z = rand() % zVerts;
				if (grid[x][yVerts - 1][z] == 1) continue;
				for (int y = yVerts - 1; y >= 0; y--)
				{
					if (grid[x][y][z] == 1)// || y == 0)
					{
						//spawn tree
						double x_ = ((double)x) / xVerts;
						double y_ = ((double)y) / yVerts;
						double z_ = ((double)z) / zVerts;

						std::shared_ptr<GameObjectColor> tree(new GameObjectColor);
						tree->transform.position.x = scale.x * ((pos.x + x_) * chunkSize.x + Gen::random() * meshDensity);
						tree->transform.position.y = scale.y * (y_ * chunkSize.y + Gen::random() * meshDensity) + Gen::random() * 20.0 + 20.0;
						tree->transform.position.z = scale.z * ((pos.y + z_) * chunkSize.z + Gen::random() * meshDensity);
						tree->transform.setRotation(0, (rand() % 360), 0);
						//tree->transform.scale.y = Gen::random() * 2.0 + 0.5;

						double scale = ((rand() % 1000) / 1000.0) * 1.0 + 0.6;
						tree->transform.scale.y = ((rand() % 1000) / 1000.0) * 1.0 + 1.0;
						tree->transform.scale *= scale;

						tree->collidable = false;
						tree->mesh = treeMesh;
						tree->shader = this->shader;

						//	int purple = rand() % 120 + 60;
						//	int r = rand() % 24;
						//	int g = rand() % (purple * 2 / 3);
						//	int b = rand() % 30;
						//	tree->setFillColor(purple + r, g, purple + b);

						int grey = rand() % 60 + 60;
						tree->setFillColor(grey, grey, grey);

						//	int r = rand() % 20 + 10;
						//	int g = rand() % 40 + 190;
						//	int b = rand() % 20 + 130;
						//	tree->setFillColor(r, g, b);

						//	tree->setFillColor(0, 255, 0);
						chunk->addChild(tree);
						break;
					}
				}
			}
		}
		//	grid[3][2][3] = true;
		//	grid[3][3][3] = true;
		//	grid[2][2][3] = true;
		//	grid[2][3][3] = true;
		//	grid[2][3][4] = true;
		//	grid[3][2][2] = false;
		//	grid[2][3][2] = false;
		//	grid[2][2][3] = false;
		//
		//	grid[6][7][7] = false;
		//	grid[7][6][7] = false;
		//	grid[7][7][6] = false;
		//
		//	grid[3][7][7] = false;
		//	grid[2][6][7] = false;
		//	grid[2][7][6] = false;
		//
		//	grid[3][2][7] = false;
		//	grid[2][3][7] = false;
		//	grid[2][2][6] = false;
		//	grid[5][5][5] = true;
		//	grid[5][5][6] = true;
		//	grid[5][6][5] = true;
		//	grid[6][6][5] = true;
		//
		//	grid[6][6][6] = true;
		//	grid[6][7][7] = true;
		//	grid[6][7][6] = true;
		//	grid[7][6][6] = true;
		//
		//
		//	grid[3][7][8] = true;
		//	grid[3][6][8] = true;
		//
		//	//grid[6][8][3] = true;
		//	grid[7][8][3] = true;

		Bounds bounds(glm::vec3(0, 0, 0), glm::vec3(scale.x * chunkSize.x, scale.y * chunkSize.y, scale.z * chunkSize.z));
		chunk->mesh = ColorMesh::meshFromVertexGrid(grid, bounds, 0, 0, 255);
		chunk->octTree->build(chunk, 4, 2);
	}

	//chunk->setFillColor(rand() % 255, rand() % 255, rand() % 255);
	return chunk;
}

void Terrain::update()
{
/*
	for (const auto &p : chunkMap)
	{
		std::shared_ptr<Chunk> chunk = p.second;
		if (chunk->toAdd)
		{
			chunk->mesh->bindArrays();

			chunk->toAdd = false;
		}
		if (chunk->toDelete)
		{
			chunkMap.erase(chunk->key);
		}
	}
	*/

	chunkManagement();
}

void Terrain::renderFunc(Camera & camera, glm::mat4 parentTransform)
{
	for (const auto &p : chunkMap)
	{
		p.second->renderFunc(camera, parentTransform);
	}

//	this->GameObjectColor::renderFunc(camera);

//	for (int i = 0; i < chunks.size(); i++)
//	{
//		chunks[i]->render(camera);
//	}
}

double Terrain::heightFunction(double y)
{
	return (y - normalizedGroundHeight) * 2.0;
	//if (y <= normalizedGroundHeight)
	//{
	//	double t = (y - normalizedGroundHeight) / (1.0 - normalizedGroundHeight);
	//	return t * t * t;
	//}
	//else
	//{
	//	double t = (y - normalizedGroundHeight) / (normalizedGroundHeight);
	//	return t * t * t;
	//}
	//return 0.0;
}

void Terrain::addNew(glm::vec2 pos)
{
	std::pair<double, double> point(pos.x, pos.y);
	if (chunkMap.find(point) == chunkMap.end())
	{
		//add a new one
		chunkMap[point] = generateChunk(pos, point);
	}
}

void Terrain::cullOld(glm::vec2 center)
{
	for (auto p = chunkMap.cbegin(); p != chunkMap.cend();)
	{
		std::shared_ptr<Chunk> chunk = p->second;
		double dX = fabs(chunk->key.first - center.x);
		double dZ = fabs(chunk->key.second - center.y);
		if (dX > chunkDistance + 1 || dZ > chunkDistance + 1)
		{
			chunkMap.erase(p++);
		}
		else
		{
			p++;
		}
	}

}

void Terrain::chunkManagement()
{

	glm::vec2 pos = glm::vec2(chunkTarget->getPosition().x / scale.x, chunkTarget->getPosition().z / scale.z);
	pos.x /= chunkSize.x;
	pos.y /= chunkSize.z;
	pos = floor(pos);
	for (int x = -chunkDistance; x <= chunkDistance; x++)
	{
		for (int z = -chunkDistance; z <= chunkDistance; z++)
		{
			addNew(pos + glm::vec2(x, z));
		}
	}
    cullOld(pos);
}

std::vector<std::shared_ptr<Triangle>> Terrain::collide(std::shared_ptr<Collider> collider)
{

	//get the proper collision chunk using the velocity
	glm::vec3 nVel = glm::normalize(collider->velocity);
	nVel *= collider->radius;
	glm::vec2 pos = glm::vec2((collider->transform.position.x) / scale.x, (collider->transform.position.z) / scale.z);
	pos.x /= chunkSize.x;
	pos.y /= chunkSize.z;
	pos = floor(pos);
	std::pair<double, double> point(pos.x, pos.y);

	//	glm::vec2 pos1 = glm::vec2((*collider->x + nVel.x) / scale.x, (*collider->z + nVel.z) / scale.z);
	//	pos1.x /= chunkSize.x;
	//	pos1.y /= chunkSize.z;
	//	pos1 = floor(pos1);
	//	std::pair<double, double> point1(pos1.x, pos1.y);


	//	std::vector<std::shared_ptr<Triangle>> out;

	//	if (point1 != point)
	//	{
	//		if (chunkMap.find(point1) != chunkMap.end())
	//		{
	//			std::shared_ptr<Chunk> chunk = chunkMap.find(point1)->second;
	//			std::shared_ptr<OctTree> octTree = chunk->octTree;
	//			std::vector<std::shared_ptr<Triangle>> temp = octTree->collide(collider);
	//			for (int i = 0; i < temp.size(); i++)
	//				out.push_back(temp[i]);
	//		}
	//	}

	if (chunkMap.find(point) != chunkMap.end())
	{
	//	std::cout << pos.x << " : " << pos.y << " \n";
		std::shared_ptr<Chunk> chunk = chunkMap.find(point)->second;
		std::shared_ptr<OctTree> octTree = chunk->octTree;
	//	std::cout << octTree << "\n";
		return octTree->collide(collider);
		//std::vector<std::shared_ptr<Triangle>> temp = octTree->collide(collider);
		//for (int i = 0; i < temp.size(); i++)
		//	out.push_back(temp[i]);
	}
	else
	{
		return std::vector<std::shared_ptr<Triangle>>();
	}


	//return out;

	/* * /
	if (chunk != nullptr)
	{
		std::shared_ptr<OctTree> octTree = chunk->octTree;// chunkMap.find(point)->second->octTree;
		if(octTree)
		return octTree->collide(collider);
	}
	else
	{
		return std::vector<std::shared_ptr<Triangle>>();
	}*/
}

void Terrain::chunkManagementLoop()
{
	while (chunkLoop)
	{
		if (chunkTarget != NULL) 
		{
			chunkManagement();
		}
	}
}

void Terrain::setTarget(std::shared_ptr<Entity> ent)
{
	chunkTarget = ent;
}

bool Terrain::ground(double x, double y, double z)
{
	//double xC = noise.octaveNoise(y, z, 6);
	//double yC = noise.octaveNoise(z, x, 6);// *0.5 + 0.5;// +2.0 * (y * 0.5 + 0.5);
	//double zC = noise.octaveNoise(x, y, 6);
	//double c = noise.octaveNoise(x, y, z, 6);
	//
	//double value = (c + xC + yC + zC) * 0.25 + 6.0 * heightFunction(y);
	//if (y < 0.0)
	//	std::cout << y << "\n";

	double height = 2.0 * (y - 0.5);

	double flat = 0.1 * noise.octaveNoise0_1(x/10.0, z/10.0, 6);
	double noisy = 0.2 * noise.octaveNoise0_1(x, y, z, 6);
	double fallOff = 5.0 * heightFunction(y);

	double expectedHeight = noise.octaveNoise0_1(x / 10.0, z/10.0, 6);
	double canyon = 2.0 * noise.octaveNoise(x, y, z, 6);
	double mountainValue = noise.octaveNoise0_1(x / 60.0, z / 60.0, 5);

	double value = noisy;// flat + fallOff;// +noisy;// *(flat + noisy);// height + flat;// +hills;//     flat * 0.5 + hills * 0.2;// +fallOff * 0.3;
	//height
	//return value < 0.0;
	//return y + curValue < expectedHeight;
	return mountainValue * (canyon) + height < 0.0; // expectedHeight + curValue + height < 0.0;
}

double Terrain::ground(double x, double z)
{
	//double xC = noise.octaveNoise(y, z, 6);
	//double yC = noise.octaveNoise(z, x, 6);// *0.5 + 0.5;// +2.0 * (y * 0.5 + 0.5);
	//double zC = noise.octaveNoise(x, y, 6);
	//double c = noise.octaveNoise(x, y, z, 6);
	//
	//double value = (c + xC + yC + zC) * 0.25 + 6.0 * heightFunction(y);
	//if (y < 0.0)
	//	std::cout << y << "\n";

	//double height = 2.0 * (y - 0.5);

	double flat = 0.1 * noise.octaveNoise0_1(x / 10.0, z / 10.0, 6);
	//double noisy = 0.2 * noise.octaveNoise0_1(x, y, z, 6);
	//double fallOff = 5.0 * heightFunction(y);

	double expectedHeight = noise.octaveNoise0_1(x / 10.0, z / 10.0, 6);
	//double canyon = 2.0 * noise.octaveNoise(x, y, z, 6);
	double mountainValue = noise.octaveNoise0_1(x / 60.0, z / 60.0, 5);
	double noisy = noise.octaveNoise0_1(x / 3.0, z / 3.0, 6);


	//double value = noisy;// flat + fallOff;// +noisy;// *(flat + noisy);// height + flat;// +hills;//     flat * 0.5 + hills * 0.2;// +fallOff * 0.3;
	//height
	//return value < 0.0;
	//return y + curValue < expectedHeight;
	return noisy;// 2.0 * mountainValue + expectedHeight + 0.3 * noisy;
}

void Terrain::closeThread()
{
	chunkLoop = false;
}

// =============================== Chunk ==============================

Chunk::Chunk()
{
	toDelete = false;
	toAdd = true;
	octTree = std::shared_ptr<OctTree>(new OctTree);
}

Chunk::~Chunk()
{

}

#pragma once
#include "Headers.h"
#include "Geometry.h"

class Mesh
{
protected:

	void bindIndexVBO(std::vector<GLuint> indexes);
	void bindVertexAttribVBO(int attrib, int size, std::vector<GLfloat> data);

	void bindVAO();
	void unbindVAO();
	void clearBuffers();
	
	std::vector<GLuint> bufferAttribs;
	bool hasBound;
public:
	Mesh();
	~Mesh();

	void render();
	void recalculateBounds();
	void setUpVAO();

	std::vector<std::shared_ptr<Triangle>> toTriangles();
	std::vector<std::shared_ptr<Triangle>> toTriangles(glm::mat4 transform);
	GLuint VertexArrayID;
	GLuint elementbufferID;
	std::vector<GLuint> indexBuffer;
	std::vector<GLfloat> vertexBuffer;
	std::vector<GLfloat> normalBuffer;
	std::vector<GLuint> bufferIDs;
	Bounds bounds;
	int indexCount;
	int vertexCount;
	int numVBOs;
	virtual void bindArrays();
//	static std::vector<int> vaos;
//	static std::vector<int> vbos;
};

class ColorMesh : public Mesh
{
private:
	static std::vector<std::vector<double>> cubeMarchList;
	static int hashVerts(bool * verts);
	static std::vector<std::shared_ptr<Triangle>> getTrianglesFromGrid(bool * verts, glm::vec3 center, glm::vec3 scale);
	static void rectifyNormals(std::vector<std::shared_ptr<Triangle>> faces, glm::vec3 normal);

public:
	~ColorMesh();
//	GLuint VertexBufferID;
//	GLuint NomralBufferID;
	GLuint ColorBufferID;
	std::vector<GLfloat> colorBuffer;
	static void recolorMesh(std::shared_ptr<ColorMesh> mesh, int r, int g, int b);
	void bindArrays();
	static std::shared_ptr<ColorMesh> triangle();
	static std::shared_ptr<ColorMesh> loadFromFile(std::string fileName);
	static std::shared_ptr<ColorMesh> recolorNonGray(std::shared_ptr<ColorMesh> mesh);
	static std::shared_ptr<ColorMesh> meshFromTriangles(std::vector<std::shared_ptr<Triangle>> & faces, int r, int g, int b);
	static std::shared_ptr<ColorMesh> meshFromTrianglesUnbound(std::vector<std::shared_ptr<Triangle>> & faces, int r, int g, int b);
	static std::shared_ptr<ColorMesh> meshFromVertexGrid(std::vector<std::vector<std::vector<bool>>> & grid, Bounds & bounds, int r, int g, int b);
	static std::shared_ptr<ColorMesh> meshFromVertexGrid(std::vector<std::vector<double>> & grid, Bounds & bounds, int r, int g, int b);
	static std::shared_ptr<ColorMesh> applyMatrixToMesh(std::shared_ptr<ColorMesh> & mesh, glm::mat4 matrix);
};

class TextureMesh : public Mesh
{
public:
//	GLuint NomralBufferID;
	GLuint uvBufferID;
//	std::vector<GLfloat> normalBuffer;
	std::vector<GLfloat> uvBuffer;
	
	static std::shared_ptr<TextureMesh> loadFromFile(std::string fileName);
};

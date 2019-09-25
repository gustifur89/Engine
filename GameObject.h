#pragma once
#include "Headers.h"
#include "Mesh.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void addChild(std::shared_ptr<GameObject> gameObject);

	void render(Camera& camera);
	virtual void renderFunc(Camera& camera);

	void renderShadow(std::vector<std::shared_ptr<Light>> lights, std::shared_ptr<Shader> shader, int location);
	virtual void renderShadowFunc(std::shared_ptr<Shader> shader, int location);

	Bounds getWorldSpaceBounds();

	std::vector<std::shared_ptr<GameObject>> children;
	bool visible;
	bool persistentVisible;
	bool collidable;
	Transform transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
};

/*
class GameObjectEmpty : public GameObject
{
public:
	//GameObjectEmpty();
	//~GameObjectEmpty();

	void renderFunc(Camera& camera);
};*/

class GameObjectColor : public GameObject
{
public:
	GameObjectColor();
	~GameObjectColor();
	glm::mat4 colorMatrix;
	
	void setFillColor(int r, int g, int b);
	void renderFunc(Camera& camera);
	void renderShadowFunc(std::shared_ptr<Shader> shader, int location);
	std::shared_ptr<ColorShader> shader;
};

class GameObjectTexture : public GameObject
{
public:
	GameObjectTexture();
	~GameObjectTexture();

	void renderFunc(Camera& camera);
	void renderShadowFunc(std::shared_ptr<Shader> shader, int location);

	std::shared_ptr<Texture> texture;
	std::shared_ptr<TextureShader> shader;
};

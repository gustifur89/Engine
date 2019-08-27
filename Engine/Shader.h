#pragma once
#include "Headers.h"
#include "Texture.h"

class Shader
{
protected:
	void loadShader_(std::string vertexFile, std::string fragmentFile);

public:
	Shader();
	~Shader();

	static std::shared_ptr<Shader> loadShader(std::string vertexFile, std::string fragmentShader);
	static std::shared_ptr<Shader> loadShader(std::string fileName);

	void useShader();

	GLint getUniformLocation(std::string uniformName);

	void loadFloat(int location, float value);

	void loadVector(int location, glm::vec3 vector);

	void loadBoolean(int location, bool value);

	void loadTexture(int location, int textureID);

	void loadMatrix(int location, glm::mat4 matrix);

	void loadMatrix(int location, glm::mat3 matrix);

	GLuint programID;
};

class ColorShader : public Shader
{
public:
	int mvp, nm, cm, light_loc;
	glm::mat4 colorMatrix;
	glm::vec3 light;
	ColorShader();
	~ColorShader();

	static std::shared_ptr<ColorShader> loadShader(std::string fileName);

	void setLight(glm::vec3 light);
	void setLightInternal(glm::vec3 light);
	void setMatrixes(glm::mat4 MVP, glm::mat4 NM, glm::mat4 colorMatrix = glm::mat4(1.0));
};

class TextureShader : public Shader
{
public:
	int mvp, nm, light_loc, texLoc;
	glm::mat4 colorMatrix;
	glm::vec3 light;
	TextureShader();
	~TextureShader();

	static std::shared_ptr<TextureShader> loadShader(std::string fileName);
	void setTexture(std::shared_ptr<Texture> texture);
	void setLight(glm::vec3 light);
	void setLightInternal(glm::vec3 light);
	void setMatrixes(glm::mat4 MVP, glm::mat4 NM);
};

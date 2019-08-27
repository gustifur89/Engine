#include "Shader.h"

// ========================================== Shader ================================

Shader::Shader()
{
}

std::shared_ptr<Shader> Shader::loadShader(std::string vertexFile, std::string fragmentFile)
{
	std::shared_ptr<Shader> out(new Shader());
	out->loadShader_(vertexFile, fragmentFile);
	return out;
}

std::shared_ptr<Shader> Shader::loadShader(std::string fileName)
{
	std::shared_ptr<Shader> out(new Shader());
	out->loadShader_(fileName, fileName);
	return out;
}

void Shader::loadShader_(std::string vertexFile, std::string fragmentFile)
{
	std::string vertex_file_path = std::string("src/shaders/") + vertexFile + std::string(".vs");
	std::string fragment_file_path = std::string("src/shaders/") + fragmentFile + std::string(".fs");
	
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	programID = glCreateProgram();
	glAttachShader(programID, VertexShaderID);
	glAttachShader(programID, FragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(programID, VertexShaderID);
	glDetachShader(programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void Shader::useShader()
{
	glUseProgram(programID);
}

GLint Shader::getUniformLocation(std::string uniformName)
{
	return glGetUniformLocation(programID, uniformName.c_str());
}

void Shader::loadFloat(int location, float value)
{
	glUniform1f(location, value);
}

void Shader::loadVector(int location, glm::vec3 vector)
{
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::loadBoolean(int location, bool value)
{
	float toLoad = 0;
	if (value) toLoad = 1;
	glUniform1f(location, toLoad);
}

void Shader::loadMatrix(int location, glm::mat4 matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::loadMatrix(int location, glm::mat3 matrix)
{
	glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::loadTexture(int location, int textureID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(location, 0);
}

Shader::~Shader()
{
}

// ========================================== ColorShader ================================

ColorShader::ColorShader()
{
//	glDetachShader(progId, vertId);
//	glDetachShader(progId, fragId);
//	glDeleteProgram(progId);
//	glDeleteShader(vertId);
//	glDeleteShader(fragId);
	light = glm::vec3(1, 1, 1);
	glm::normalize(light);
}

std::shared_ptr<ColorShader> ColorShader::loadShader(std::string fileName)
{
	//Shader::loadShader(fileName);
	std::shared_ptr<ColorShader> out(new ColorShader());
	out->Shader::loadShader_(fileName, fileName);
	out->mvp = out->getUniformLocation("MVP");
	//out->mv = out->getUniformLocation("MV");
	out->cm = out->getUniformLocation("ColorMatrix");
	out->loadMatrix(out->cm, glm::mat4(1.0));
	out->nm = out->getUniformLocation("NM");
	out->light_loc = out->getUniformLocation("light");
	return out;
}

void ColorShader::setLight(glm::vec3 light)
{
	this->light = light;
	glm::normalize(this->light);
}

void ColorShader::setLightInternal(glm::vec3 light)
{
	loadVector(light_loc, light);
}

void ColorShader::setMatrixes(glm::mat4 MVP, glm::mat4 NM, glm::mat4 colorMatrix)
{
	loadMatrix(mvp, MVP);
//	loadMatrix(mv, MV);
	loadMatrix(cm, colorMatrix);
	loadMatrix(nm, NM);
}

ColorShader::~ColorShader()
{
}

// ========================================== TextureShader ================================

TextureShader::TextureShader()
{
	//	glDetachShader(progId, vertId);
	//	glDetachShader(progId, fragId);
	//	glDeleteProgram(progId);
	//	glDeleteShader(vertId);
	//	glDeleteShader(fragId);
	light = glm::vec3(1, 1, 1);
	glm::normalize(light);
}

std::shared_ptr<TextureShader> TextureShader::loadShader(std::string fileName)
{
	std::shared_ptr<TextureShader> out(new TextureShader());
	out->Shader::loadShader_(fileName, fileName);
	out->mvp = out->getUniformLocation("MVP");
	out->nm = out->getUniformLocation("NM");
	out->texLoc = out->getUniformLocation("tex");
	out->light_loc = out->getUniformLocation("light");
	return out;
}

void TextureShader::setTexture(std::shared_ptr<Texture> texture)
{
	loadTexture(texLoc, texture->textureID);
}

void TextureShader::setLight(glm::vec3 light)
{
	this->light = light;
	glm::normalize(this->light);
}

void TextureShader::setLightInternal(glm::vec3 light)
{
	loadVector(light_loc, light);
}

void TextureShader::setMatrixes(glm::mat4 MVP, glm::mat4 NM)
{
	loadMatrix(mvp, MVP);
	loadMatrix(nm, NM);
}

TextureShader::~TextureShader()
{
}

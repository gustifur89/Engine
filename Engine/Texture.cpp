#include "Texture.h"



Texture::Texture()
{

}


Texture::~Texture()
{

}

std::shared_ptr<Texture> Texture::loadFromFile(std::string fileName)
{
	fileName = std::string("src/textures/") + fileName;
	int width, height, nrChannels;
	unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);

	std::shared_ptr<Texture> texture(new Texture());
	
	// Create one OpenGL texture
	glGenTextures(1, &texture->textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture->textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return texture;
	
}

#include "Texture.h"
#include <iostream>
#include "stb_image.h"

Texture::Texture(const char* fileName, GLenum texture_type, GLint texture_unit, GLenum format)
{
	if (id)
	{
		glDeleteTextures(1, &id);
	}

	type = texture_type;
	textureUnit = texture_unit;
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);

	glGenTextures(1, &id);
	glBindTexture(type, id);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(type, id);
}

void Texture::unBind()
{
	glActiveTexture(0);
	glBindTexture(type, 0);
}

void Texture::loadFromFile(const char* fileName, GLenum format)
{
	if (id)
	{
		glDeleteTextures(1, &id);
	}

	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);

	glGenTextures(1, &id);
	glBindTexture(type, id);

	glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture from file" << std::endl;
	}
	stbi_image_free(data);
}

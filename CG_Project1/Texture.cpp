#include "Texture.h"
#include <iostream>
#include "stb_image.h"

Texture::Texture(const char* fileName, GLenum type)
	: id(0), width(0), height(0), nrChannels(0), type(type), loaded(false), path(fileName)
{
	// Print loading attempt
	std::cout << "Loading texture: " << fileName << std::endl;

	// Generate texture ID first
	glGenTextures(1, &id);
	std::cout << "Generated new texture ID: " << id << std::endl;

	// Load image data
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glBindTexture(type, id);

		// Set texture parameters
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Determine format based on number of channels
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		GLenum internalFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;

		// Upload texture data
		glTexImage2D(type, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(type);

		// Success message
		std::cout << "Texture loaded successfully: " << fileName << std::endl;
		std::cout << "Dimensions: " << width << "x" << height << std::endl;
		std::cout << "Channels: " << nrChannels << std::endl;
		std::cout << "Texture ID: " << id << std::endl;

		loaded = true;

		glBindTexture(type, 0);
	}
	else
	{
		std::cout << "Failed to load texture: " << fileName << std::endl;
		std::cout << "STB Error: " << stbi_failure_reason() << std::endl;
		glDeleteTextures(1, &id);
		id = 0;
	}

	// Free image data
	stbi_image_free(data);
}

Texture::~Texture()
{
	cleanup();
}

void Texture::bind(GLint textureUnit) const
{
	if (loaded)
	{
		//std::cout << "Binding texture (ID: " << id << ") to unit " << textureUnit << std::endl;
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(type, id);

		GLint boundTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
		//std::cout << "Verified bound texture: " << boundTexture << " on unit " << textureUnit << std::endl;
	}
	else
	{
		std::cout << "Warning: Attempting to bind invalid texture (ID: " << id << ")" << std::endl;
	}
}

void Texture::unbind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(type, 0);
}

void Texture::cleanup()
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
		id = 0;
	}
	loaded = false;
}

#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>

class Texture
{
private:
	GLuint id;
	int width;
	int height;
	int nrChannels;
	GLenum type;
	bool loaded;
	std::string path;

	void cleanup();

public:
	Texture(const char* fileName, GLenum type = GL_TEXTURE_2D);
	~Texture();

	void bind(GLint textureUnit = 0) const;

	void unbind() const;

	// Getters
	GLuint getId() const { return id; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	GLenum getType() const { return type; }
	bool isLoaded() const { return loaded; }
	const std::string& getPath() const { return path; }
};

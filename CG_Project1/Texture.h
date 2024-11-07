#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

class Texture
{
private:
	GLuint id;
	int width;
	int height;
	int nrChannels;
	unsigned int type;
	GLint textureUnit;

public:
	Texture(const char* fileName, GLenum type, GLint textureUnit, GLenum format);
	~Texture();

	inline GLuint getID() const { return id; };

	void bind();

	void unBind();
	
	inline GLint getTextureUnit() const { return textureUnit; }

	void loadFromFile(const char* fileName, GLenum format);
};


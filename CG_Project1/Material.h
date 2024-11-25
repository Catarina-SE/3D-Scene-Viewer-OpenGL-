#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Shader.h"
#include "Texture.h"

class Material
{
private:
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	std::string diffuseMapPath;
	std::unique_ptr<Texture> diffuseTexture;
	GLuint textureId;

public:
	Material(const std::string& name);

	// Setters
	void setName(const std::string& name) { this->name = name; }
	void setAmbient(const glm::vec3& ambient) { this->ambient = ambient; }
	void setDiffuse(const glm::vec3& diffuse) { this->diffuse = diffuse; }
	void setSpecular(const glm::vec3& specular) { this->specular = specular; }
	void setShininess(float shininess) { this->shininess = shininess; }
	void setDiffuseMap(const std::string& path);

	// Getters
	const std::string& getName() const { return name; }
	const glm::vec3& getAmbient() const { return ambient; }
	const glm::vec3& getDiffuse() const { return diffuse; }
	const glm::vec3& getSpecular() const { return specular; }
	float getShininess() const { return shininess; }
	const std::string& getDiffuseMapPath() const { return diffuseMapPath; }
	GLuint getTextureId() const { return textureId; }
	bool hasTexture() const { return textureId != 0; }

	// Apply material to shader
	void apply(Shader& shader) const;

	~Material();
};
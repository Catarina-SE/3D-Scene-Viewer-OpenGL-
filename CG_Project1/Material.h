#pragma once

#include <string>
#include <memory>
#include "Shader.h"
#include "Texture.h"

class Material {
private:
	std::string name;
	std::unique_ptr<Texture> diffuseTexture;
	GLuint textureId;

public:
	Material(const std::string& name) : name(name), textureId(0) {}
	void setDiffuseMap(const std::string& path);
	void apply(Shader& shader) const;
	bool hasTexture() const { return textureId != 0; }
	std::string& getName() { return name; }
};
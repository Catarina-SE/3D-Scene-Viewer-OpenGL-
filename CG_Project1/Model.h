#pragma once

#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class Model
{
private:
	std::vector<Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::string directory;

	void loadOBJ(const char* objPath);
	void loadMaterials(const char* objPath);

public:
	Model(const char* objPath);
	~Model();

	void draw(Shader& shader);
};
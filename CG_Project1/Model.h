#pragma once

#include <vector>
#include <string>
#include <map>
#include "Mesh.h"

class Model {
private:
	std::vector<Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::string directory;
	void loadOBJ(const char* objPath);
	void loadMaterials(const char* mtlPath);

public:
	Model(const char* objPath);
	~Model();
	void draw(Shader& shader, const glm::mat4& modelMatrix);

	Material* getMaterial(const std::string& materialName);
};

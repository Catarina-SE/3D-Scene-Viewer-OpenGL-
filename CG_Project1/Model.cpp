#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const char* objPath) {
	loadOBJ(objPath);
}

Model::~Model() {
	for (Mesh* mesh : meshes) delete mesh;
	for (auto& pair : materials) delete pair.second;
}

void Model::draw(Shader& shader, const glm::mat4& modelMatrix)
{
	shader.setMat4("model", modelMatrix);
	for (Mesh* mesh : meshes) {
		mesh->draw(shader);
	}
}

void Model::loadOBJ(const char* objPath) {
	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_texcoords;
	std::vector<Vertex> currentVertices;
	std::vector<unsigned int> currentIndices;
	Material* currentMaterial = nullptr;

	directory = std::string(objPath);
	size_t lastSlash = directory.find_last_of("/\\");
	if (lastSlash != std::string::npos) directory = directory.substr(0, lastSlash);

	std::ifstream file(objPath);
	if (!file.is_open()) {
		std::cout << "Failed to open OBJ file: " << objPath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "mtllib") {
			std::string mtlFilename;
			iss >> mtlFilename;
			loadMaterials((directory + "/" + mtlFilename).c_str());
		}
		else if (prefix == "v") {
			glm::vec3 position;
			iss >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (prefix == "vt") {
			glm::vec2 texcoord;
			iss >> texcoord.x >> texcoord.y;
			temp_texcoords.push_back(texcoord);
		}
		else if (prefix == "usemtl") {
			if (!currentVertices.empty()) {
				meshes.push_back(new Mesh(currentVertices, currentIndices, currentMaterial));
				currentVertices.clear();
				currentIndices.clear();
			}
			std::string materialName;
			iss >> materialName;
			currentMaterial = materials[materialName];
		}
		else if (prefix == "f") {
			std::string vertex;
			std::vector<Vertex> faceVertices;

			while (iss >> vertex) {
				std::istringstream viss(vertex);
				std::string index_str;
				int v_idx = -1, t_idx = -1;

				std::getline(viss, index_str, '/');
				if (!index_str.empty()) v_idx = std::stoi(index_str) - 1;

				std::getline(viss, index_str, '/');
				if (!index_str.empty()) t_idx = std::stoi(index_str) - 1;

				Vertex vert;
				vert.Position = temp_positions[v_idx];
				if (t_idx >= 0 && !temp_texcoords.empty())
					vert.TexCoords = temp_texcoords[t_idx];

				faceVertices.push_back(vert);
			}

			for (size_t i = 1; i < faceVertices.size() - 1; i++) {
				currentVertices.push_back(faceVertices[0]);
				currentVertices.push_back(faceVertices[i]);
				currentVertices.push_back(faceVertices[i + 1]);
				currentIndices.push_back(currentVertices.size() - 3);
				currentIndices.push_back(currentVertices.size() - 2);
				currentIndices.push_back(currentVertices.size() - 1);
			}
		}
	}

	if (!currentVertices.empty()) {
		meshes.push_back(new Mesh(currentVertices, currentIndices, currentMaterial));
	}
}

void Model::loadMaterials(const char* mtlPath) {
	std::ifstream file(mtlPath);
	if (!file.is_open()) return;

	Material* currentMaterial = nullptr;
	std::string line;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "newmtl") {
			std::string materialName;
			iss >> materialName;
			currentMaterial = new Material(materialName);
			materials[materialName] = currentMaterial;
		}
		else if (currentMaterial && prefix == "map_Kd") {
			std::string texturePath;
			iss >> texturePath;
			currentMaterial->setDiffuseMap(directory + "/" + texturePath);
		}
	}
}
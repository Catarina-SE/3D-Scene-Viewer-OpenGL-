#include "Model.h"

#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const char* objPath)
{
	loadOBJ(objPath);
}

Model::~Model()
{
	for (Mesh* mesh : meshes) {
		delete mesh;
	}
	meshes.clear();

	// Clean up materials
	for (auto& pair : materials) {
		delete pair.second;
	}
	materials.clear();

}

void Model::draw(Shader& shader)
{
	for (Mesh* mesh : meshes) {
		mesh->draw(shader);
	}
}

void Model::loadOBJ(const char* objPath)
{
	directory = std::string(objPath);
	size_t lastSlash = directory.find_last_of("/\\");
	if (lastSlash != std::string::npos) {
		directory = directory.substr(0, lastSlash);
	}

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_texcoords;
	std::vector<glm::vec3> temp_normals;

	std::vector<Vertex> currentVertices;
	std::vector<unsigned int> currentIndices;
	Material* currentMaterial = nullptr;

	std::ifstream file(objPath);
	if (!file.is_open())
	{
		std::cout << "Failed to open OBJ file: " << objPath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "mtllib")
		{
			std::string mtlFilename;
			iss >> mtlFilename;
			std::string mtlPath = directory + "/" + mtlFilename;
			if (!std::ifstream(mtlPath).good())
			{
				mtlPath = directory + "/wall/" + mtlFilename;
			}
			std::cout << "Attempting to load MTL from: " << mtlPath << std::endl;
			loadMaterials(mtlPath.c_str());
		}

		else if (prefix == "usemtl")
		{
			// If we have vertices, create a new mesh with the current material
			if (!currentVertices.empty()) {
				meshes.push_back(new Mesh(currentVertices, currentIndices, currentMaterial));
				currentVertices.clear();
				currentIndices.clear();
			}

			std::string materialName;
			iss >> materialName;

			// Look up material in our map
			auto it = materials.find(materialName);
			currentMaterial = (it != materials.end()) ? it->second : nullptr;
		}
		else if (prefix == "v")
		{
			glm::vec3 position;
			iss >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (prefix == "vt")
		{
			glm::vec2 texcoord;
			iss >> texcoord.x >> texcoord.y;
			// texcoord.y = 1.0f - texcoord.y; // Already flipped by stbi_set_flip_vertically_on_load(true) in Texture.cpp
			temp_texcoords.push_back(texcoord);
		}
		else if (prefix == "vn")
		{
			glm::vec3 normal;
			iss >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (prefix == "f")
		{
			std::string vertex;
			std::vector<Vertex> faceVertices;  // Store all vertices for this face

			// First, collect all vertices of the face
			while (iss >> vertex)
			{
				std::istringstream viss(vertex);
				std::string index_str;
				int index_count = 0;
				int v_idx = -1, t_idx = -1, n_idx = -1;

				while (std::getline(viss, index_str, '/'))
				{
					if (!index_str.empty())
					{
						int idx = std::stoi(index_str) - 1;
						if (index_count == 0) v_idx = idx;
						else if (index_count == 1) t_idx = idx;
						else if (index_count == 2) n_idx = idx;
					}
					index_count++;
				}

				// Create vertex
				Vertex vert;
				vert.Position = temp_positions[v_idx];
				if (t_idx >= 0 && !temp_texcoords.empty())
					vert.TexCoords = temp_texcoords[t_idx];
				if (n_idx >= 0 && !temp_normals.empty())
					vert.Normal = temp_normals[n_idx];

				faceVertices.push_back(vert);
			}

			// Triangulate the face (assuming it's convex)
			for (size_t i = 1; i < faceVertices.size() - 1; i++)
			{
				// Add first vertex of the face
				currentVertices.push_back(faceVertices[0]);
				currentIndices.push_back(currentVertices.size() - 1);

				// Add vertex i+1
				currentVertices.push_back(faceVertices[i]);
				currentIndices.push_back(currentVertices.size() - 1);

				// Add vertex i+2
				currentVertices.push_back(faceVertices[i + 1]);
				currentIndices.push_back(currentVertices.size() - 1);
			}
		}
	}

	// Add the last mesh if we have vertices

	if (!currentVertices.empty())
	{
		meshes.push_back(new Mesh(currentVertices, currentIndices, currentMaterial));
	}

	file.close();
}



void Model::loadMaterials(const char* mtlPath)
{
	std::ifstream file(mtlPath);
	if (!file.is_open())
	{
		std::cout << "Failed to open MTL file: " << mtlPath << std::endl;
		return;
	}

	Material* currentMaterial = nullptr;
	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string prefix;
		iss >> prefix;

		if (prefix == "newmtl")
		{
			std::string materialName;
			iss >> materialName;
			currentMaterial = new Material(materialName);
			materials[materialName] = currentMaterial;  // Store in map with name as key
		}

		else if (currentMaterial != nullptr)
		{
			if (prefix == "Ka")
			{
				float x, y, z;
				iss >> x >> y >> z;
				currentMaterial->setAmbient(glm::vec3(x, y, z));
			}

			else if (prefix == "Kd")
			{
				float x, y, z;
				iss >> x >> y >> z;
				std::cout << "Setting diffuse color: " << x << ", " << y << ", " << z << std::endl;
				currentMaterial->setDiffuse(glm::vec3(x, y, z));
			}

			else if (prefix == "Ks")
			{
				float x, y, z;
				iss >> x >> y >> z;
				currentMaterial->setSpecular(glm::vec3(x, y, z));
			}

			else if (prefix == "Ns")
			{
				float shininess;
				iss >> shininess;
				currentMaterial->setShininess(shininess);
			}

			else if (prefix == "map_Kd")
			{
				std::string texturePath;
				iss >> texturePath;
				std::string fullPath = directory + "/" + texturePath;

				if (!std::ifstream(fullPath).good())
				{
					fullPath = directory + "/wall/" + texturePath;
				}

				std::cout << "Loading texture: " << fullPath << std::endl;
				currentMaterial->setDiffuseMap(fullPath);
			}
		}
	}

	file.close();
}

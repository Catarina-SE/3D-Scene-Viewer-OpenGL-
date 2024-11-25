#pragma once

#include <vector>
#include "Vertex.h"
#include "Material.h"
#include "Shader.h"

class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Material* material;
	GLuint vao, vbo, ebo;

	void setupMesh();

public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, Material* material);
	~Mesh();

	void draw(Shader& shader);

	void addVertex(const Vertex& vertex);

	void addIndex(unsigned int index);

	void setMaterial(Material* material);
};
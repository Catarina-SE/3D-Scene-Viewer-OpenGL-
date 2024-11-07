#pragma once
#include <iostream>
#include "glm/glm.hpp"

class Mesh
{
public:
	glm::vec3 Position;
	glm::vec3 Vertices;
	glm::vec3 Normals;
	glm::vec3 textCoords;

	Mesh();
	~Mesh();


};


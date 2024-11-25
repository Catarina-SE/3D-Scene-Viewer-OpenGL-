#pragma once

#include <glm/glm.hpp>

class Vertex
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Normal;

	Vertex() = default;

	Vertex(const glm::vec3& pos, const glm::vec2& tex = glm::vec2(0.0f), const glm::vec3& norm = glm::vec3(0.0f))
		: Position(pos), TexCoords(tex), Normal(norm) {}
};
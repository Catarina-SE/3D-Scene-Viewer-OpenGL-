#pragma once

#include <glm/glm.hpp>

class Vertex {
public:
	glm::vec3 Position;
	glm::vec2 TexCoords;

	Vertex() = default;
	Vertex(const glm::vec3& pos, const glm::vec2& tex = glm::vec2(0.0f))
		: Position(pos), TexCoords(tex) {}
};
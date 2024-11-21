
#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <glad/glad.h>


// OpenGL Math Libs
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Owns Libs
//#include <Vertex.h>

struct Vertex {
    float x, y, z;
    float nx, ny, nz; // Optional: normals for lighting
    float u, v;       // Optional: texture coordinates

    Vertex(float x, float y, float z) : x(x), y(y), z(z), nx(0), ny(0), nz(0), u(0), v(0) {}
    Vertex(float x, float y, float z, float nx, float ny, float nz) : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(0), v(0) {}
    Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) : x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v) {}
};


static std::vector<Vertex> loadOBJ(const char* file_name)
{
	//Vertex Portions
	std::vector<glm::fvec3> vertex_positions;
	std::vector<glm::fvec2> vertex_texcoords;
	std::vector<glm::fvec3> vertex_normals;

	// Face Vector
	std::vector<GLuint> vertex_position_indicies;
	std::vector<GLuint> vertex_texcoord_indicies;
	std::vector<GLuint> vertex_normal_indicies;

	// Vertex Array
	std::vector<Vertex> vertices;

	std::stringstream ss;
	std::ifstream in_file(file_name);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLuint temp_glint;


	// Check for errors opening file
	if (!in_file.is_open())
	{
		throw "ERROR::OBJLoader::COULD NOT OPEN FILE.";
	}

	while (std::getline(in_file, line))
	{
		// Get the prefix of the line
		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#")
		{

		}

		if (prefix == "o")
		{

		}

		if (prefix == "s")
		{

		}
		
		else if (prefix == "use_mtl")
		{

		}

		if (prefix == "v") // Vertex Position
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
		}
		if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{

		}
		else
		{

		}

		// DEBUG
		std::cout << line << "\n";
		std::cout << "Nr of Vertices: " << vertex_positions.size() << "\n";
	}

	// Loaded Successfully
	return vertices;
}









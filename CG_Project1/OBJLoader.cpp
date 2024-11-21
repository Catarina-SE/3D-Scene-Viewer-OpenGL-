#include "OBJLoader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals)
{
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file = nullptr;
	errno_t err = fopen_s(&file, path, "r");
	if (err != 0 || file == NULL) {
		printf("Impossible to open the file!\n");
		return false;
	}

	while (1) {
		char lineHeader[128];
		int res = fscanf_s(file, "%s", lineHeader, (unsigned)_countof(lineHeader));
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::vector<unsigned int> vertexIndexList;
			std::vector<unsigned int> uvIndexList;
			std::vector<unsigned int> normalIndexList;

			while (true) {
				unsigned int vertexIndex, uvIndex, normalIndex;
				int matches = fscanf_s(file, "%d/%d/%d", &vertexIndex, &uvIndex, &normalIndex);

				if (matches == 3) {
					vertexIndexList.push_back(vertexIndex);
					uvIndexList.push_back(uvIndex);
					normalIndexList.push_back(normalIndex);
				}
				else {
					// Clear the rest of the line if there are no more indices
					fscanf_s(file, "%*[^\n]\n");
					break;
				}
			}

			// Triangulate the face if it has more than three vertices
			if (vertexIndexList.size() >= 3) {
				// For faces with 4 or more vertices (n-gons), split them into triangles
				for (size_t i = 1; i < vertexIndexList.size() - 1; i++) {
					// First triangle (v1, vi, vi+1)
					vertexIndices.push_back(vertexIndexList[0]);
					vertexIndices.push_back(vertexIndexList[i]);
					vertexIndices.push_back(vertexIndexList[i + 1]);

					if (!uvIndexList.empty()) {
						uvIndices.push_back(uvIndexList[0]);
						uvIndices.push_back(uvIndexList[i]);
						uvIndices.push_back(uvIndexList[i + 1]);
					}

					if (!normalIndexList.empty()) {
						normalIndices.push_back(normalIndexList[0]);
						normalIndices.push_back(normalIndexList[i]);
						normalIndices.push_back(normalIndexList[i + 1]);
					}
				}
			}
			else {
				printf("File can't be read by our parser: Unsupported face format\n");
				fclose(file);
				return false;
			}
		}
	}

	fclose(file);

	// Assemble vertices, uvs, and normals with bounds checking and debug prints
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		if (vertexIndex > 0 && vertexIndex <= temp_vertices.size()) {
			out_vertices.push_back(temp_vertices[vertexIndex - 1]);
		}
		else {
			printf("Vertex index %d out of range! Max valid index: %zu\n", vertexIndex, temp_vertices.size());
			return false;
		}
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		if (uvIndex > 0 && uvIndex <= temp_uvs.size()) {
			out_uvs.push_back(temp_uvs[uvIndex - 1]);
		}
		else {
			printf("UV index %d out of range! Max valid index: %zu\n", uvIndex, temp_uvs.size());
			return false;
		}
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		if (normalIndex > 0 && normalIndex <= temp_normals.size()) {
			out_normals.push_back(temp_normals[normalIndex - 1]);
		}
		else {
			printf("Normal index %d out of range! Max valid index: %zu\n", normalIndex, temp_normals.size());
			return false;
		}
	}

	return true;
}

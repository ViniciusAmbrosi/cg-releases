#pragma once

#define _CRT_SECURE_NO_DEPRECATE

#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class ObjectModel
{
public:
	std::vector < glm::vec3 > vertices;
	std::vector < glm::vec2 > uvs;
	std::vector < glm::vec3 > normals;
	glm::vec3 originalColor;

	ObjectModel() {}
	ObjectModel(const char* path, int sizeFactor, int xDeslocation, glm::vec3 color)
	{
		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;
		originalColor = color;

		FILE* file = fopen(path, "r");
		if (file == NULL) {
			printf("Impossible to open the file !\n");
			return;
		}

		while (1) {
			char lineHeader[128];

			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.

			// else : parse lineHeader
			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

				vertex.x = (vertex.x / sizeFactor) + xDeslocation;
				vertex.y = vertex.y / sizeFactor;
				vertex.z = vertex.z / sizeFactor;

				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);

				uv.x = (uv.x / sizeFactor) + xDeslocation;
				uv.y = uv.y / sizeFactor;

				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;

				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);

				normal.x = (normal.x / sizeFactor) + xDeslocation;
				normal.y = normal.y / sizeFactor;
				normal.z = normal.z / sizeFactor;

				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					break;
				}

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
			}
		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			unsigned int vertexIndex = vertexIndices[i];
			unsigned int normalIndex = normalIndices[i];

			glm::vec3 vertex = temp_vertices[vertexIndex - 1];
			glm::vec3 normalVertex = temp_normals[normalIndex - 1];

			vertices.push_back(vertex);
			vertices.push_back(normalVertex);
		}

		for (unsigned int i = 0; i < uvIndices.size(); i++) {
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 vertex = temp_uvs[uvIndex - 1];
			uvs.push_back(vertex);
		}

		for (unsigned int i = 0; i < normalIndices.size(); i++) {
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 vertex = temp_normals[normalIndex - 1];
			normals.push_back(vertex);
		}
	}

	void UpdateModelColor()
	{
		UpdateColor(glm::vec3(0.0f, 0.2f, 1.0f));
	}

	void ResetColor()
	{
		UpdateColor(originalColor);
	}

	std::vector < glm::vec3 > getVertices() {
		return vertices;
	}

	std::vector < glm::vec2 > getUvs() {
		return uvs;
	}

	std::vector < glm::vec3 > getNormals() {
		return normals;
	}

private:
	void UpdateColor(glm::vec3 color)
	{
		for (int i = 1; i <= vertices.size(); i = i + 2)
		{
			vertices[i].r = color.r;
			vertices[i].g = color.g;
			vertices[i].b = color.b;
		}
	};
};


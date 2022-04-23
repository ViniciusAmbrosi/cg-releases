#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include "Geometry.cpp"
#include "Model.cpp"
#include "Shader.cpp"

using namespace std;

class Program
{
public:

	bool debugging = false;
	GLuint ShaderProgram;
	std::vector < Geometry > Geometries;

	Program (Shader vertexShader, Shader fragmentShader)
	{
		GLint success;
		GLchar infoLog[512];

		ShaderProgram = glCreateProgram();
		glAttachShader(ShaderProgram, vertexShader.GetShader());
		glAttachShader(ShaderProgram, fragmentShader.GetShader());
		glLinkProgram(ShaderProgram);

		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
		ValidateProgramCreationResult(ShaderProgram, success, infoLog);

		glDeleteShader(vertexShader.GetShader());
		glDeleteShader(fragmentShader.GetShader());
	}

	void DrawAllGeometries()
	{
		for (Geometry geometry : Geometries)
		{
			geometry.DrawElements();
		}
	}

	void DeleteAllGeometries()
	{
		for (Geometry geometry : Geometries)
		{
			geometry.DeleteElements();
		}
	}

	Geometry SetupGeometryForModel(Model model)
	{
		DisplayModelProperties(model);

		GLuint VBO, VAO;

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,
			model.getVertices().size() * sizeof(glm::vec3),
			model.getVertices().data(),
			GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*) sizeof(glm::vec3));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		Geometry newGeometry = Geometry(VAO, VBO, model.getVertices().size() / 3);

		Geometries.push_back(newGeometry);
		
		return newGeometry;
	}

	Geometry SetupGeometryForArray(std::vector <GLfloat> vector)
	{
		GLuint VBO, VAO;

		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER,
			vector.size() * sizeof(GLfloat),
			vector.data(),
			GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		Geometry newGeometry = Geometry(VAO, VBO, vector.size() / 3);

		Geometries.push_back(newGeometry);

		return newGeometry;
	}

	const GLuint GetProgram()
	{
		return ShaderProgram;
	}

private:

	void DisplayModelProperties(Model model)
	{
		if (debugging)
		{
			printf("Number of vertices %d | Number of triangles %d | Number of squares %d \n",
				model.getVertices().size(),
				model.getVertices().size() / 3,
				model.getVertices().size() / 6);

			for (int i = 0; i < model.getVertices().size(); ++i) {
				printf("Vertice x: %f y: %f z: %f \n", model.getVertices()[i].x, model.getVertices()[i].y, model.getVertices()[i].z);
			}

			for (int i = 0; i < model.getUvs().size(); ++i) {
				printf("Uvs x: %f y: %f  \n", model.getUvs()[i].x, model.getUvs()[i].y);
			}

			for (int i = 0; i < model.getNormals().size(); ++i) {
				printf("Normal x: %f y: %f z: %f  \n", model.getNormals()[i].x, model.getNormals()[i].y, model.getNormals()[i].z);
			}
		}
	}

	void ValidateProgramCreationResult(GLuint programId, GLint success, GLchar* resultMessage)
	{
		if (!success)
		{
			glGetShaderInfoLog(programId, 512, NULL, resultMessage);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << resultMessage << endl;
		}
	}
};


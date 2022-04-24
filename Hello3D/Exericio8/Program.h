#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include "Geometry.cpp"
#include "ObjectModel.cpp"
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

	Geometry SetupGeometryForModel(ObjectModel model)
	{
		DisplayModelProperties(model);

		Geometry newGeometry = Geometry(model.getVertices().size() / 3, model);
		newGeometry.BindGeometry();

		Geometries.push_back(newGeometry);
		
		return newGeometry;
	}

	Geometry SetupGeometryForArray(std::vector <GLfloat> vector)
	{
		Geometry newGeometry = Geometry(vector.size() / 3);
		newGeometry.BindGeometry();

		Geometries.push_back(newGeometry);

		return newGeometry;
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

	std::vector <Geometry> GetGeometries()
	{
		return Geometries;
	}

	const GLuint GetProgram()
	{
		return ShaderProgram;
	}

private:

	void DisplayModelProperties(ObjectModel model)
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


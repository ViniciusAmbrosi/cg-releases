#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include "Shader.cpp"

using namespace std;

class Program
{
public:
	GLuint shaderProgram;

	Program (Shader vertexShader, Shader fragmentShader)
	{
		GLint success;
		GLchar infoLog[512];

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader.GetShader());
		glAttachShader(shaderProgram, fragmentShader.GetShader());
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		ValidateProgramCreationResult(shaderProgram, success, infoLog);

		glDeleteShader(vertexShader.GetShader());
		glDeleteShader(fragmentShader.GetShader());
	}

	const GLuint GetProgram()
	{
		return shaderProgram;
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


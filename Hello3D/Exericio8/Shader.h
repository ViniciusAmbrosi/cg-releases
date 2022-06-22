#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../../libs/glad/include/glad/glad.h"

using namespace std;

class Shader
{
public:
	string shaderContent;
	GLuint shaderId;

	Shader(const GLchar* shaderPath, GLenum shaderType)
	{
		GLint success;
		GLchar infoLog[512];

		ReadShaderSourceCode(shaderPath);
		auto shaderReference = shaderContent.c_str();

		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &shaderReference, NULL);

		glCompileShader(shaderId);

		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		ValidateShaderCreationResult(shaderId, success, infoLog);
	}
	
	void ReadShaderSourceCode(const GLchar* shaderPath)
	{
		string shaderCode;
		ifstream shaderFile;

		shaderFile.exceptions(ifstream::badbit);
		try
		{
			shaderFile.open(shaderPath);

			stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();

			shaderFile.close();

			shaderContent = shaderStream.str();
		}
		catch (ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		}
	}

	const GLuint GetShader()
	{
		return shaderId;
	}

	void ValidateShaderCreationResult(GLuint shaderId, GLint success, GLchar* resultMessage)
	{
		if (!success)
		{
			glGetShaderInfoLog(shaderId, 512, NULL, resultMessage);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << resultMessage << endl;
		}
	}
};


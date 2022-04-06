#pragma once

#include <glad/glad.h>

class Geometry
{
public:
	GLuint VAO;
	GLuint VBO;
	int Triangles;

	Geometry(GLuint vao, GLuint vbo, int triangles)
	{
		VAO = vao;
		VBO = vbo;
		Triangles = triangles;
	}

	void DrawElements()
	{
		glBindVertexArray(VAO);
		DrawElements(VAO, Triangles);
	}

	void DeleteElements()
	{
		glDeleteVertexArrays(1, &VAO);
	}

private:
	void DrawElements(GLuint vao, int size) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, size * 3);
	}
};
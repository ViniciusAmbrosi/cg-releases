#pragma once

#include "ObjectModel.cpp"
#include <glad/glad.h>

class Geometry
{
public:
	ObjectModel model;

	GLuint VAO;
	GLuint VBO;
	int Triangles;
	bool selected;

	Geometry(int triangles, ObjectModel modelParam)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		Triangles = triangles;
		selected = false;
		model = modelParam;
	}

	Geometry(int triangles)
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		Triangles = triangles;
		selected = false;
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

	void UpdateGeometryColor()
	{
		model.UpdateModelColor();
		BindGeometry();
	}

	void ResetModelColor()
	{
		model.ResetColor();
		BindGeometry();
	}

	void BindGeometry() 
	{
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
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)sizeof(glm::vec3));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

private:
	void DrawElements(GLuint vao, int size) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, size * 3);
	}
};
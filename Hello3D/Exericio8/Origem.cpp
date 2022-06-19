﻿using namespace std;

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include "Vectors.h"
#include "ObjectModel.cpp"
#include "Program.cpp"
#include "Shader.cpp"
#include "Geometry.cpp"
#include "Model.h"
#include "CallbackHandler.h"

#include <iostream>

const GLuint WIDTH = 1000, HEIGHT = 1000;

CallbackHandler callbackHandler = CallbackHandler(45.0, WIDTH, HEIGHT);

Program setupProgram()
{
	Shader vertexShaderSource = Shader("Resources/ShaderFiles/vertexShader.vs", GL_VERTEX_SHADER);
	Shader fragmentShaderSource = Shader("Resources/ShaderFiles/fragmentShader.fs", GL_FRAGMENT_SHADER);
		
	return Program(vertexShaderSource, fragmentShaderSource);
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	callbackHandler.scrollHandler.HandleScrollCallback(yoffset);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	callbackHandler.mouseHandler.HandleMouseCallback(xpos, ypos, cameraFront);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	callbackHandler.keyboardHandler.HandleKeyboardCallback(key, action, cameraPos, cameraFront, cameraUp);
}

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Computação Gráfica", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Program program = setupProgram();

	MeshShader meshShader("Resources/ShaderFiles/vertexShader.vs", "Resources/ShaderFiles/fragmentShader.fs");
	Model ourModel("Resources/Models/Pokemon/Pikachu.obj");
	Model ourSecondModel("Resources/Models/Cube/cube.obj");

	meshShader.use();

	float ka, kd, ks, n;

	glm::mat4 model = glm::mat4(1); 
	GLint modelLocation = glGetUniformLocation(meshShader.ID, "model");

	glm::mat4 view;
	GLint viewLocation = glGetUniformLocation(meshShader.ID, "view");

	glm::mat4 projection;
	GLint projLocation = glGetUniformLocation(meshShader.ID, "projection");

	//GLint objectColorLocation = glGetUniformLocation(program.GetProgram(), "finalColor"); - removido por causa das texturas
	GLint lightColorLocation = glGetUniformLocation(meshShader.ID, "lightColor");
	GLint lightPosLocation = glGetUniformLocation(meshShader.ID, "lightPos");
	GLint viewPosLocation = glGetUniformLocation(meshShader.ID, "viewPos");

	GLint kaLocation = glGetUniformLocation(meshShader.ID, "ka");
	GLint kdLocation = glGetUniformLocation(meshShader.ID, "kd");
	GLint ksLocation = glGetUniformLocation(meshShader.ID, "ks");
	GLint nLocation = glGetUniformLocation(meshShader.ID, "n");

	projection = glm::perspective(glm::radians(callbackHandler.scrollHandler.GetFov()), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//glUniform3f(objectColorLocation, 1.0f, 0.0f, 1.0f); - removido por causa das texturas
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLocation, 0.0, 10.0, -5.0);
	glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform1f(kaLocation, 1.0);
	glUniform1f(kdLocation, 0.45);
	glUniform1f(ksLocation, 0.9);
	glUniform1f(nLocation, 32);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	model = glm::mat4(1);
	while (!glfwWindowShouldClose(window))
	{
		callbackHandler.keyboardHandler.UpdateDelta();

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		callbackHandler.HandleAllKeyboardActions(model, program.Geometries);

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(callbackHandler.scrollHandler.GetFov()), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(viewPosLocation, cameraPos.x, cameraPos.y, cameraPos.z);

		meshShader.use();
		meshShader.setMat4("projection", projection);
		meshShader.setMat4("view", view);
		meshShader.setMat4("model", model);

		ourModel.Draw(meshShader);
		//ourSecondModel.Draw(meshShader);

		glfwSwapBuffers(window);
	}

	program.DeleteAllGeometries();

	glfwTerminate();
	return 0;
}

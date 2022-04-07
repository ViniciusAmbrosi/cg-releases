using namespace std;

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include "Vectors.h"
#include "Model.cpp"
#include "Program.cpp"
#include "Shader.cpp"
#include "Geometry.cpp"

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KeyboardCallback.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Program setupProgram()
{
	Shader vertexShaderSource = Shader("Resources/ShaderFiles/vertexShader.vs", GL_VERTEX_SHADER);
	Shader fragmentShaderSource = Shader("Resources/ShaderFiles/fragmentShader.fs", GL_FRAGMENT_SHADER);

	return Program(vertexShaderSource, fragmentShaderSource);
}

const GLuint WIDTH = 1000, HEIGHT = 1000;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool firstMouse = true;
float lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0;
float yaw = -90.0, pitch = 0.0;
float fov = 45.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Computação Gráfica", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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

	Model cube = Model("Resources/Models/Cube/cube.obj", 1, 0);
	Model malePikachu = Model("Resources/Models/Pokemon/Pikachu.obj", 2, 0);
	Model femalePikachu = Model("Resources/Models/Pokemon/PikachuF.obj", 18, 2);
	
	Geometry CubeGeometry = program.SetupGeometryForArray(CubeVector);
	Geometry FloorGeometry = program.SetupGeometryForArray(FloorVector);
	Geometry PikachuGeometry = program.SetupGeometryForModel(malePikachu);
	Geometry FemalePikachuGeometry = program.SetupGeometryForModel(femalePikachu);

	glUseProgram(program.GetProgram());

	glm::mat4 model = glm::mat4(1); 
	GLint modelLoc = glGetUniformLocation(program.GetProgram(), "model");

	glm::mat4 view;
	GLint viewLoc = glGetUniformLocation(program.GetProgram(), "view");

	glm::mat4 projection;
	GLint projLoc = glGetUniformLocation(program.GetProgram(), "projection");

	projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1);
		if (moveDirection == MoveDirection::Up)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::Down)
		{
			model = glm::rotate(model, angle, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::Left)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::Right)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, -1.0f, 0.0f));
		}

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		program.DrawAllGeometries();

		glfwSwapBuffers(window);
	}

	program.DeleteAllGeometries();

	glfwTerminate();
	return 0;
}

void resetMovement() {
	moveDirection = MoveDirection::Idle;
}
void resetEverything() {
	resetMovement();

	cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	
	HandleKeyboardCallback(key, action, deltaTime, cameraPos, cameraFront, cameraUp, moveDirection);
}
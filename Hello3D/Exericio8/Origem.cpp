using namespace std;

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include "Vectors.h"
#include "Model.cpp"
#include "Program.cpp"
#include "Shader.cpp"
#include "Geometry.cpp"

#include "CallbackHandler.h"

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

	//Model cube = Model("Resources/Models/Cube/cube.obj", 1, 0);
	Model malePikachu = Model("Resources/Models/Pokemon/Pikachu.obj", 2, -1, glm::vec3(1.0f,1.0f,0.0f));
	Model femalePikachu = Model("Resources/Models/Pokemon/PikachuF.obj", 18, 1, glm::vec3(0.8f, 0.8f, 0.0f));
	
	//Geometry CubeGeometry = program.SetupGeometryForArray(CubeVector);
	//Geometry FloorGeometry = program.SetupGeometryForArray(FloorVector);
	Geometry PikachuGeometry = program.SetupGeometryForModel(malePikachu);
	Geometry FemalePikachuGeometry = program.SetupGeometryForModel(femalePikachu);

	glUseProgram(program.GetProgram());

	glm::mat4 model = glm::mat4(1); 
	GLint modelLocation = glGetUniformLocation(program.GetProgram(), "model");

	glm::mat4 view;
	GLint viewLocation = glGetUniformLocation(program.GetProgram(), "view");

	glm::mat4 projection;
	GLint projLocation = glGetUniformLocation(program.GetProgram(), "projection");

	projection = glm::perspective(glm::radians(callbackHandler.scrollHandler.GetFov()), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));

		program.DrawAllGeometries();

		glfwSwapBuffers(window);
	}

	program.DeleteAllGeometries();

	glfwTerminate();
	return 0;
}

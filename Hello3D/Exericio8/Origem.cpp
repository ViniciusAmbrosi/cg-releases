using namespace std;

#define _CRT_SECURE_NO_DEPRECATE

#include "Program.cpp"
#include "Shader.cpp"
#include "Model.cpp"

#include <string>
#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool debugging = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Program setupProgram()
{
	Shader vertexShaderSource = Shader("Resources/ShaderFiles/vertexShader.vs", GL_VERTEX_SHADER);
	Shader fragmentShaderSource = Shader("Resources/ShaderFiles/fragmentShader.fs", GL_FRAGMENT_SHADER);

	return Program(vertexShaderSource, fragmentShaderSource);
}

int setupGeometry();
int setupFloorGeometry();
int setupModelGeometry(Model threeDimensionalModel);

const GLuint WIDTH = 1000, HEIGHT = 1000;

bool rotateX = false, rotateY = false, rotateZ = false, rotateLeft = false, rotateRight = false, rotateUp = false, rotateDown = false;
bool front = false, back = false, left = false, right = false, top = false, shouldMove = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool firstMouse = true;
float lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0;
float yaw = -90.0, pitch = 0.0;
float fov = 45.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void drawElement(GLuint vao, int size) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, size * 3);
}

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

	Model cube = Model("Resources/Models/Cube/cube.obj");
	Model malePikachu = Model("Resources/Models/Pokemon/Pikachu.obj");
	Model femalePikachu = Model("Resources/Models/Pokemon/PikachuF.obj");
	
	GLuint VAO_BASIC_CUBE = setupGeometry();
	GLuint VAO_BASIC_FLOOR = setupFloorGeometry();
	GLuint VAO_MALE_PIKACHU = setupModelGeometry(malePikachu);
	GLuint VAO_FEMALE_PIKACHU = setupModelGeometry(femalePikachu);

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
		if (rotateX && rotateUp)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (rotateX && rotateDown)
		{
			model = glm::rotate(model, angle, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else if (rotateY && rotateLeft)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (rotateY && rotateRight)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, -1.0f, 0.0f));
		}

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//glBindVertexArray(VAO_BASIC_CUBE);
		//glBindVertexArray(VAO_FLOOR);
		//glBindVertexArray(VAO_FEMALE_PIKACHU);
		glBindVertexArray(VAO_MALE_PIKACHU);

		//drawElement(VAO_BASIC_CUBE, 12);
		//drawElement(VAO_FLOOR, 2);
		//drawElement(VAO_MODEL, threeDimensionalModel.getVertices().size() / 3);
		drawElement(VAO_MALE_PIKACHU, femalePikachu.getVertices().size() / 3);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO_MALE_PIKACHU);

	glfwTerminate();
	return 0;
}

void resetMovement() {
	rotateX = false;
	rotateY = false;
	rotateZ = false;

	rotateUp = false;
	rotateDown = false;
	rotateLeft = false;
	rotateRight = false;
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
	float cameraSpeed = 200.0f * deltaTime;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		resetMovement();

		rotateX = true;
		rotateUp = true;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		resetMovement();

		rotateX = true;
		rotateDown = true;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		resetMovement();

		rotateY = true;
		rotateLeft = true;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		resetMovement();

		rotateY = true;
		rotateRight = true;
	}

	if (key == GLFW_KEY_W && action != GLFW_RELEASE)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_S && action != GLFW_RELEASE)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_A && action != GLFW_RELEASE)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (key == GLFW_KEY_D && action != GLFW_RELEASE)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	//front - GREEN
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 5.0f); // distance from the block
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // where the camera is looking to
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
	}
	//back - YELLOW
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 0.0f, -5.0f); // distance from the block
		cameraFront = glm::vec3(0.0f, 0.0f, 4.0f); // where the camera is looking to
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
	}
	//left - PINK
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(-5.0f, 0.0f, 0.0f); // distance from the block
		cameraFront = glm::vec3(1.0f, 0.0f, 0.0f); // where the camera is looking to
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
	}
	//right - BLUE
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(5.0f, 0.0f, 0.0f); // distance from the block
		cameraFront = glm::vec3(-4.0f, 0.0f, 0.0f); // where the camera is looking to
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
	}
	//top - RED
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 5.0f, 0.0f); // distance from the block
		cameraFront = glm::vec3(0.0f, -4.0f, 0.0f); // what is the "top" of the camera
		cameraUp = glm::vec3(1.0f, 0.0f, 0.0f); // what is the "top" of the camera
	}

	//reset every movement
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		resetEverything();
	}
}

int setupGeometry()
{
	GLfloat vertices[] = {
		//bottom CDC' + DD'C'
		-0.5, -0.5, -0.5, 0.0, 0.0, 0.0,
		-0.5, -0.5,  0.5, 0.0, 0.0, 0.0,
		0.5, -0.5, -0.5,  0.0, 0.0, 0.0,

		-0.5, -0.5,  0.5, 0.0, 0.0, 0.0,
		0.5, -0.5,  0.5,  0.0, 0.0, 0.0,
		0.5, -0.5, -0.5,  0.0, 0.0, 0.0,

		//top ABA' + BB'A'
		-0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
		-0.5, 0.5,  0.5, 1.0, 0.0, 0.0,
		0.5, 0.5, -0.5,  1.0, 0.0, 0.0,

		-0.5, 0.5,  0.5, 1.0, 0.0, 0.0,
		0.5, 0.5,  0.5,  1.0, 0.0, 0.0,
		0.5, 0.5, -0.5,  1.0, 0.0, 0.0,

		//front ABC + BCA
		-0.5,  0.5, 0.5, 0.0, 1.0, 0.0,
		-0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
		0.5, -0.5, 0.5,  0.0, 1.0, 0.0,

		-0.5,  0.5, 0.5, 0.0, 1.0, 0.0,
		0.5,  0.5, 0.5,  0.0, 1.0, 0.0,
		0.5, -0.5, 0.5,  0.0, 1.0, 0.0,

		//back A'B'C' + B'C'A'
		-0.5,  0.5, -0.5, 1.0, 1.0, 0.0,
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, -0.5, -0.5,  1.0, 1.0, 0.0,

		-0.5, 0.5, -0.5, 1.0, 1.0, 0.0,
		0.5,  0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, -0.5, -0.5, 1.0, 1.0, 0.0,

		//left ACC' + AA'C'
		-0.5, 0.5,  -0.5, 1.0, 0.0, 1.0,
		-0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		-0.5, -0.5, 0.5,  1.0, 0.0, 1.0,

		-0.5, 0.5,  -0.5, 1.0, 0.0, 1.0,
		-0.5, 0.5,  0.5,  1.0, 0.0, 1.0,
		-0.5, -0.5, 0.5,  1.0, 0.0, 1.0,

		//right BDD' + BB'D'
		0.5,  0.5, -0.5, 0.0, 0.0, 1.0,
		0.5, -0.5, -0.5, 0.0, 0.0, 1.0,
		0.5, -0.5, 0.5,  0.0, 0.0, 1.0,

		0.5, 0.5, -0.5, 0.0, 0.0, 1.0,
		0.5, 0.5, 0.5,  0.0, 0.0, 1.0,
		0.5, -0.5, 0.5, 0.0, 0.0, 1.0,

		//2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		//-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
		// 2.0, -1.0, -2.0, 0.0, 0.0, 0.0,

		// 2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		//-2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		//-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}
int setupFloorGeometry()
{
	GLfloat floor[] = {
		 2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
		 2.0, -1.0, -2.0, 0.0, 0.0, 0.0,

		 2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		-2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}
int setupModelGeometry(Model threeDimensionalModel) {

	if (debugging)
	{
		printf("Number of vertices %d | Number of triangles %d | Number of squares %d \n",
			threeDimensionalModel.getVertices().size(),
			threeDimensionalModel.getVertices().size() / 3,
			threeDimensionalModel.getVertices().size() / 6);

		for (int i = 0; i < threeDimensionalModel.getVertices().size(); ++i) {
			printf("Vertice x: %f y: %f z: %f \n", threeDimensionalModel.getVertices()[i].x, threeDimensionalModel.getVertices()[i].y, threeDimensionalModel.getVertices()[i].z);
		}

		for (int i = 0; i < threeDimensionalModel.getUvs().size(); ++i) {
			printf("Uvs x: %f y: %f  \n", threeDimensionalModel.getUvs()[i].x, threeDimensionalModel.getUvs()[i].y);
		}

		for (int i = 0; i < threeDimensionalModel.getNormals().size(); ++i) {
			printf("Normal x: %f y: %f z: %f  \n", threeDimensionalModel.getNormals()[i].x, threeDimensionalModel.getNormals()[i].y, threeDimensionalModel.getNormals()[i].z);
		}
	}

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 
		threeDimensionalModel.getVertices().size() * sizeof(glm::vec3), 
		//&threeDimensionalModel.getVertices()[0], 
		threeDimensionalModel.getVertices().data(),
		GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Atributo posição (x, y, z)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	//Atributo cor (r, g, b)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}
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

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Program setupProgram()
{
	Shader vertexShaderSource = Shader("Resources/ShaderFiles/vertexShader.vs", GL_VERTEX_SHADER);
	Shader fragmentShaderSource = Shader("Resources/ShaderFiles/fragmentShader.fs", GL_FRAGMENT_SHADER);

	return Program(vertexShaderSource, fragmentShaderSource);
}

// Protótipos das funções
int setupGeometry();
int setupFloorGeometry();
int setupModelGeometry();

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

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Program program = setupProgram();

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();
	GLuint VAO_FLOOR = setupFloorGeometry();
	GLuint VAO_MODEL = setupModelGeometry();

	glUseProgram(program.GetProgram());

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(program.GetProgram(), "model");

	glm::mat4 view;
	GLint viewLoc = glGetUniformLocation(program.GetProgram(), "view");

	glm::mat4 projection;
	GLint projLoc = glGetUniformLocation(program.GetProgram(), "projection");
	projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	//projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// Passa seu conteúdo para o shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1);
		if (rotateX && rotateUp)
		{
			glBindVertexArray(VAO);
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (rotateX && rotateDown)
		{
			glBindVertexArray(VAO);
			model = glm::rotate(model, angle, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else if (rotateY && rotateLeft)
		{
			glBindVertexArray(VAO);
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotateY && rotateRight)
		{
			glBindVertexArray(VAO);
			model = glm::rotate(model, angle, glm::vec3(0.0f, -1.0f, 0.0f));
		}

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//glBindVertexArray(VAO);
		//glBindVertexArray(VAO_FLOOR);
		glBindVertexArray(VAO_MODEL);

		//FOCUS: This dictates how many figures will be drawn
		//drawElement(VAO, 12);
		//drawElement(VAO_FLOOR, 2);
		drawElement(VAO_MODEL, 12);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	//glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VAO_MODEL);

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
	float cameraSpeed = 10.0f * deltaTime;

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
	// FRONT FACE
	// A (top-left) -> - 0.5 |   0.5 | - 0.5
	// B (top-right) ->  0.5 |   0.5 | - 0.5
	// C (low-left) -> - 0.5 | - 0.5 | - 0.5
	// D (low-right) ->  0.5 | - 0.5 | - 0.5
	// BACK FACE
	// A' (top-left) -> - 0.5 |   0.5 | 0.5
	// B' (top-right) ->  0.5 |   0.5 | 0.5
	// C' (low-left) -> - 0.5 | - 0.5 | 0.5
	// D' (low-right) ->  0.5 | - 0.5 | 0.5

	//MEANING THAT A CUBE IS MADE OF
	//BOTTOM CDC' + DD'C'
	//TOP ABA' + BB'A'
	//FRONT ABC + BCA
	//BACK A'B'C' + B'C'A'
	//LEFT ACC' + AA'C'
	//RIGHT BDD' + BB'D'

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

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
int setupFloorGeometry()
{
	//FOCUS: For a cube there are 8 main points
	// FRONT FACE
	// A (top-left) -> - 0.5 |   0.5 | - 0.5
	// B (top-right) ->  0.5 |   0.5 | - 0.5
	// C (low-left) -> - 0.5 | - 0.5 | - 0.5
	// D (low-right) ->  0.5 | - 0.5 | - 0.5
	// BACK FACE
	// A' (top-left) -> - 0.5 |   0.5 | 0.5
	// B' (top-right) ->  0.5 |   0.5 | 0.5
	// C' (low-left) -> - 0.5 | - 0.5 | 0.5
	// D' (low-right) ->  0.5 | - 0.5 | 0.5

	//MEANING THAT A CUBE IS MADE OF
	//BOTTOM CDC' + DD'C'
	//TOP ABA' + BB'A'
	//FRONT ABC + BCA
	//BACK A'B'C' + B'C'A'
	//LEFT ACC' + AA'C'
	//RIGHT BDD' + BB'D'

	GLfloat floor[] = {
		 2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
		 2.0, -1.0, -2.0, 0.0, 0.0, 0.0,

		 2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		-2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
		-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
int setupModelGeometry() {

	Model threeDimensionalModel = Model("Resources/Models/Cube/cube.obj");

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

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, threeDimensionalModel.getVertices().size() * sizeof(glm::vec3), &threeDimensionalModel.getVertices()[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	////Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
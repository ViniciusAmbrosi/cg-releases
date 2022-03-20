/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 09/08/2021
 *
 */


#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();
int setupFloorGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
//"gl_Position = model * vec4(position, 1.0);\n" before doing projection
"gl_Position = projection * view * model * vec4(position, 1.0f);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 450\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

bool rotateX = false, rotateY = false, rotateZ = false, rotateLeft = false, rotateRight = false, rotateUp = false, rotateDown = false;
bool front = false, back = false, left = false, right = false, top = false, shouldMove = true;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void drawElement(GLuint vao, int size) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, size * 3);
}

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();
	GLuint VAO_FLOOR = setupFloorGeometry();

	glUseProgram(shaderID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shaderID, "model");

	glm::mat4 view;
	GLint viewLoc = glGetUniformLocation(shaderID, "view");

	glm::mat4 projection;
	GLint projLoc = glGetUniformLocation(shaderID, "projection");
	projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// Passa seu conteúdo para o shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_DEPTH_TEST);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
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

		view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES

		glBindVertexArray(VAO);
		glBindVertexArray(VAO_FLOOR);
		
		//FOCUS: This dictates how many figures will be drawn
		drawElement(VAO, 12);
		drawElement(VAO_FLOOR, 2);
		//glDrawArrays(GL_TRIANGLES, 0, 42);

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP

		//FOCUS: This dictates how many figures will be drawn
		//glDrawArrays(GL_POINTS, 0, 42);
		//glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
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

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float cameraSpeed = 0.05f; // adjust accordingly
	float pitch = 0, yaw = 0;

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
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	//front - GREEN
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
	}
	//back - YELLOW
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 0.0f, -5.0f);
	}
	//left - PINK
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(-5.0f, 0.0f, 0.0f);
	}
	//right - BLUE
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(5.0f, 0.0f, 0.0f);
	}
	//top - RED
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 5.0f, 0.01f);
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		cameraPos = glm::vec3(0.0f, 5.0f, 0.0f);
	}
}

//Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados

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
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados

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
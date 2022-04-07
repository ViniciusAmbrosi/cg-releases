#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class MoveDirection { Idle, Up, Down, Left, Right };

class Keyboard
{
public:

	void HandleKeyboardCallback(int key,
		int action,
		glm::vec3& cameraPos,
		glm::vec3& cameraFront,
		glm::vec3& cameraUp)
	{
		float cameraSpeed = 200.0f * deltaTime;

		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			moveDirection = MoveDirection::Up;
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			moveDirection = MoveDirection::Down;
		}
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			moveDirection = MoveDirection::Left;
		}
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			moveDirection = MoveDirection::Right;
		}

		else if (key == GLFW_KEY_W && action != GLFW_RELEASE)
		{
			cameraPos += cameraSpeed * cameraFront;
		}
		else if (key == GLFW_KEY_S && action != GLFW_RELEASE)
		{
			cameraPos -= cameraSpeed * cameraFront;
		}
		else if (key == GLFW_KEY_A && action != GLFW_RELEASE)
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		else if (key == GLFW_KEY_D && action != GLFW_RELEASE)
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		//front - GREEN
		else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			cameraPos = glm::vec3(0.0f, 0.0f, 5.0f); // distance from the block
			cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // where the camera is looking to
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
		}
		//back - YELLOW
		else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			cameraPos = glm::vec3(0.0f, 0.0f, -5.0f); // distance from the block
			cameraFront = glm::vec3(0.0f, 0.0f, 4.0f); // where the camera is looking to
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
		}
		//left - PINK
		else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		{
			cameraPos = glm::vec3(-5.0f, 0.0f, 0.0f); // distance from the block
			cameraFront = glm::vec3(1.0f, 0.0f, 0.0f); // where the camera is looking to
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
		}
		//right - BLUE
		else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		{
			cameraPos = glm::vec3(5.0f, 0.0f, 0.0f); // distance from the block
			cameraFront = glm::vec3(-4.0f, 0.0f, 0.0f); // where the camera is looking to
			cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // what is the "top" of the camera
		}
		//top - RED
		else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		{
			cameraPos = glm::vec3(0.0f, 5.0f, 0.0f); // distance from the block
			cameraFront = glm::vec3(0.0f, -4.0f, 0.0f); // what is the "top" of the camera
			cameraUp = glm::vec3(1.0f, 0.0f, 0.0f); // what is the "top" of the camera
		}

		//reset every movement
		else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		{
			ResetAllMovements(cameraPos, cameraFront, cameraUp);
		}
	}

	void UpdateDelta()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void HandleModelRotation(glm::mat4 &model)
	{
		model = glm::mat4(1);
		float angle = (GLfloat)glfwGetTime();

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
	}

private:
	MoveDirection moveDirection = MoveDirection::Idle;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	void ResetAllMovements(
		glm::vec3& cameraPos,
		glm::vec3& cameraFront,
		glm::vec3& cameraUp)
	{
		moveDirection = MoveDirection::Idle;

		cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}
};


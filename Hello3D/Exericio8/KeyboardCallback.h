#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class MoveDirection { 
	Idle, 
	RotateXPositive, //up
	RotateXNegative, //down
	RotateYPositive, //left
	RotateYNegative, //right
	RotateZPositive, //left diagonal
	RotateZNegative, //right diagonal
	TranslateXPositive, //up
	TranslateXNegative, //down
	TranslateYPositive, //left
	TranslateYNegative, //right
	TranslateZPositive, //left diagonal
	TranslateZNegative, //right diagonal

};

class Keyboard
{
public:

	void HandleKeyboardCallback(int key,
		int action,
		glm::vec3& cameraPos,
		glm::vec3& cameraFront,
		glm::vec3& cameraUp)
	{
		float cameraSpeed = 30.0f * deltaTime;

		if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::RotateXPositive);
		}
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::RotateXNegative);
		}
		else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::RotateYPositive);
		}
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::RotateYNegative);
		}
		else if (key == GLFW_KEY_DELETE && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::RotateZPositive);
		}
		else if (key == GLFW_KEY_INSERT && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::RotateZNegative);
		}
		
		if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::TranslateYPositive);
		}
		else if (key == GLFW_KEY_K && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::TranslateYNegative);
		}
		else if (key == GLFW_KEY_J && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::TranslateXNegative);
		}
		else if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::TranslateXPositive);
		}
		else if (key == GLFW_KEY_U && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::TranslateZPositive);
		}
		else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			SetMovement(MoveDirection::TranslateZNegative);
		}

		if (key == GLFW_KEY_W && action != GLFW_RELEASE)
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
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
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
		if (moveDirection == MoveDirection::RotateXPositive)
		{
			model = glm::rotate(model, movementRate, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::RotateXNegative)
		{
			model = glm::rotate(model, movementRate, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::RotateYPositive)
		{
			model = glm::rotate(model, movementRate, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::RotateYNegative)
		{
			model = glm::rotate(model, movementRate, glm::vec3(0.0f, -1.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::RotateZPositive) 
		{
			model = glm::rotate(model, movementRate, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (moveDirection == MoveDirection::RotateZNegative) 
		{
			model = glm::rotate(model, movementRate, glm::vec3(0.0f, 0.0f, -1.0f));
		}
	}

	void HandleModelTranslation(glm::mat4& model)
	{
		if (moveDirection == MoveDirection::TranslateYPositive)
		{
			model = glm::translate(model, glm::vec3(0.0f, movementRate, 0.0f));
		}
		else if (moveDirection == MoveDirection::TranslateYNegative)
		{
			model = glm::translate(model, glm::vec3(0.0f, -movementRate, 0.0f));
		}
		else if (moveDirection == MoveDirection::TranslateXPositive)
		{
			model = glm::translate(model, glm::vec3(movementRate, 0.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::TranslateXNegative)
		{
			model = glm::translate(model, glm::vec3(-movementRate, 0.0f, 0.0f));
		}
		else if (moveDirection == MoveDirection::TranslateZPositive)
		{
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, movementRate));
		}
		else if (moveDirection == MoveDirection::TranslateZNegative)
		{
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -movementRate));
		}
	}
private:
	MoveDirection moveDirection = MoveDirection::Idle;

	float deltaTime = 0.0f;	// Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame
	float movementRate = 0.05f;

	void ResetAllMovements(
		glm::vec3& cameraPos,
		glm::vec3& cameraFront,
		glm::vec3& cameraUp)
	{
		SetMovement(MoveDirection::Idle);

		cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		movementRate = 0.0f;
	}

	void SetMovement(MoveDirection moveDirectionParam) {
		moveDirection = moveDirectionParam;
	}
};


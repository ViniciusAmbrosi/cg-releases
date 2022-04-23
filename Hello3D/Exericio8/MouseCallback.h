#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mouse
{
public:

	Mouse()
	{
	}

	Mouse(GLuint width, GLuint height)
	{
		lastX = width / 2.0;
		lastY = height / 2.0;
	}

	float GetLastX() 
	{
		return lastX;
	}

	float GetLastY() 
	{
		return lastY;
	}

	void HandleMouseCallback(
		double xPos,
		double yPos,
		glm::vec3& cameraFront)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		float xoffset = xPos - lastX;
		float yoffset = lastY - yPos;
		lastX = xPos;
		lastY = yPos;

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

private:
	bool firstMouse = true;
	float yaw = -90.0;
	float pitch = 0.0;

	float lastX;
	float lastY;
};




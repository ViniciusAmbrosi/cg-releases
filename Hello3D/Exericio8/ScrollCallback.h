#pragma once

#include <GLFW/glfw3.h>

class Scroll
{
public:

	Scroll(float fov)
	{
		Fov = fov;
	}

	float GetFov()
	{
		return Fov;
	}

	void HandleScrollCallback(double& yoffset)
	{
		if (Fov >= 1.0f && Fov <= 45.0f)
			Fov -= yoffset;
		if (Fov <= 1.0f)
			Fov = 1.0f;
		if (Fov >= 45.0f)
			Fov = 45.0f;
	}

private:
	float Fov;
};
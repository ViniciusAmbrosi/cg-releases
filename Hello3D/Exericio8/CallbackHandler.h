#pragma once

#include "KeyboardCallback.h"
#include "ScrollCallback.h"
#include "MouseCallback.h"

class CallbackHandler
{
public:
	Scroll scrollHandler;
	Mouse mouseHandler;
	Keyboard keyboardHandler;

	CallbackHandler(float fov, GLuint width, GLuint height)
	{
		scrollHandler = Scroll(fov);
		mouseHandler = Mouse(width, height);
		keyboardHandler = Keyboard();
	}

	void HandleAllKeyboardActions(glm::mat4& model, std::vector< Model >& geometries)
	{ 
		keyboardHandler.HandleModelRotation(model);
		keyboardHandler.HandleModelTranslation(model);
		keyboardHandler.HandleModelScale(model);
		keyboardHandler.HandleModelSelection(model, geometries);
	}
};
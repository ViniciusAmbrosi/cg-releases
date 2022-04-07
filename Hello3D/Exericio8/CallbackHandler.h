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
};
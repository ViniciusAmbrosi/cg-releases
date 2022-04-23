#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objectColor;

out vec4 finalColor;

void main()
{
	//...pode ter mais linhas de c�digo aqui!
	//gl_Position = model * vec4(position, 1.0); before doing projection

	gl_Position = projection * view * model * vec4(position, 1.0f);
	finalColor = vec4(color, 1.0);
};

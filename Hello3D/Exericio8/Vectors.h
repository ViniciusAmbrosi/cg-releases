#pragma once;

#include <vector>
#include <glad/glad.h>

std::vector <GLfloat> FloorVector = {
	 2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
	-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
	 2.0, -1.0, -2.0, 0.0, 0.0, 0.0,

	 2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
	-2.0, -1.0,  2.0, 0.0, 0.0, 0.0,
	-2.0, -1.0, -2.0, 0.0, 0.0, 0.0,
};
std::vector <GLfloat> CubeVector = {
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
};
// fixModel.cpp

#include "stdafx.h"
#include "fixModel.h"

std::vector<GLfloat> vectorVertices;

GLuint indices[3*2] = { 0, 1, 2,
						0, 2, 3};

GLfloat groundVertex[3*6] = {	-2.0, 0.0, -2.0,
								2.0, 0.0, -2.0,
								2.0, 0.0, 2.0,
								-2.0, 0.0, -2.0,
								2.0, 0.0, 2.0,
								-2.0, 0.0, 2.0 };

GLfloat vertices[36*3] = {
						-0.5,-0.5,-0.5,	// 0
						-0.5,0.5,-0.5,		// 3
						0.5,0.5,-0.5,		// 2
						-0.5,-0.5,-0.5,	// 0
						0.5,0.5,-0.5,		// 2
						0.5,-0.5,-0.5,		// 1

						0.5,0.5,-0.5,		// 2
						-0.5,0.5,-0.5,		// 3
						-0.5,0.5,0.5,		// 7
						0.5,0.5,-0.5,		// 2
						-0.5,0.5,0.5,		// 7
						0.5,0.5,0.5,		// 6

						-0.5,-0.5,-0.5,	// 0
						-0.5,-0.5,0.5,		// 4
						-0.5,0.5,0.5,		// 7
						-0.5,-0.5,-0.5,	// 0
						-0.5,0.5,0.5,		// 7
						-0.5,0.5,-0.5,		// 3

						0.5,-0.5,-0.5,		// 1
						0.5,0.5,-0.5,		// 2
						0.5,0.5,0.5,		// 6
						0.5,-0.5,-0.5,		// 1
						0.5,0.5,0.5,		// 6
						0.5,-0.5,0.5,		// 5

						-0.5,-0.5,0.5,		// 4
						0.5,-0.5,0.5,		// 5
						0.5,0.5,0.5,		// 6
						-0.5,-0.5,0.5,		// 4
						0.5,0.5,0.5,		// 6
						-0.5,0.5,0.5,		// 7

						-0.5,-0.5,-0.5,	// 0
						0.5,-0.5,-0.5,		// 1
						0.5,-0.5,0.5,		// 5
						-0.5,-0.5,-0.5,	// 0
						0.5,-0.5,0.5,		// 5
						-0.5,-0.5,0.5		// 4						
						};

GLfloat colors[36*3] = {
						1.0, 0.0, 0.0,	// Red
						1.0, 0.0, 0.0,	// Red
						1.0, 0.0, 0.0,	// Red
						1.0, 0.0, 0.0,	// Red
						1.0, 0.0, 0.0,	// Red
						1.0, 0.0, 0.0,	// Red

						0.0, 1.0, 0.0,	// Green
						0.0, 1.0, 0.0,	// Green
						0.0, 1.0, 0.0,	// Green
						0.0, 1.0, 0.0,	// Green
						0.0, 1.0, 0.0,	// Green
						0.0, 1.0, 0.0,	// Green

						0.0, 0.0, 1.0,	// Blue
						0.0, 0.0, 1.0,	// Blue
						0.0, 0.0, 1.0,	// Blue
						0.0, 0.0, 1.0,	// Blue
						0.0, 0.0, 1.0,	// Blue
						0.0, 0.0, 1.0,	// Blue

						0.0, 1.0, 1.0,	// Cyan
						0.0, 1.0, 1.0,	// Cyan
						0.0, 1.0, 1.0,	// Cyan
						0.0, 1.0, 1.0,	// Cyan
						0.0, 1.0, 1.0,	// Cyan
						0.0, 1.0, 1.0,	// Cyan

						1.0, 0.0, 1.0,	// Magenta
						1.0, 0.0, 1.0,	// Magenta
						1.0, 0.0, 1.0,	// Magenta
						1.0, 0.0, 1.0,	// Magenta
						1.0, 0.0, 1.0,	// Magenta
						1.0, 0.0, 1.0,	// Magenta

						1.0, 1.0, 0.0,	// Yellow
						1.0, 1.0, 0.0,	// Yellow
						1.0, 1.0, 0.0,	// Yellow
						1.0, 1.0, 0.0,	// Yellow
						1.0, 1.0, 0.0,	// Yellow
						1.0, 1.0, 0.0,	// Yellow
					};
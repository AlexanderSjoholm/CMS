// Model.cpp

#include "stdafx.h"
#include "Model.h"
#include "fixModel.h"

namespace my
{

Model::Model()
{

}

void Model::init(
				 GLuint _program, 
				 GLchar* _vertexAttributeName, 
				 GLchar* _colourAttributeName)
{
	program = _program;
	vertexAttributeName = _vertexAttributeName;
	colourAttributeName = _colourAttributeName;

	// Setup a VertexAttributeObject for the model
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &CBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, vertexAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, vertexAttributeName));

	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, colourAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, colourAttributeName));
}

}
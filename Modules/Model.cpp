// Model.cpp

#include "stdafx.h"
#include "Model.h"
#include "fixModel.h"


Model::Model()
{
	numberOfVertices = 0;	
	numberOfIndices = 0;
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
	//glGenBuffers(1, &CBO);
	
	glBindVertexArray(VAO);
		
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertex), groundVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, vertexAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, vertexAttributeName));
	
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, colourAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, colourAttributeName));
}

void Model::upload()
{
	// Setup a VertexAttributeObject for the model
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	glGenBuffers(1, &NBO);
	//glGenBuffers(1, &CBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size()*sizeof(GLfloat), &vertexArray.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, vertexAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, vertexAttributeName));
/*
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normalArray.size()*sizeof(GLfloat), &normalArray.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, normalAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, normalAttributeName));
	*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size()*sizeof(unsigned int), &indexArray.front(), GL_STATIC_DRAW);
}


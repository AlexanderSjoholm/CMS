// myInit.cpp

#include "stdafx.h"

#include <fstream>
#include "myInit.h"


using namespace std;

char* readFile2Char(char* file)
{
	ifstream s;
	int length;
	// Open stream to/from file
	s.open(file, ios::in);
	// Find length of file
	s.seekg(0, ios::end);
	length = (int)s.tellg();
	// Set pointer to begining of file
	s.seekg(0, ios::beg);

	char* buffer = new char[length];
	s.read(buffer, length);
	s.close();
	// Add zero termination
	buffer[length] = 0;
	
	return buffer;
}

void shaderInit(GLuint* program, std::string vertexPath, std::string fragmentPath)
{
	// Program and Shader Identifiers
	GLchar *vertexCode, *fragmentCode;
	GLuint vertexShader, fragmentShader;

	// Create shader objects
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Read shaders from external files
	//vertexCode = readFile2Char("Shaders/test.vert");
	//fragmentCode = readFile2Char("Shaders/test.frag");
	vertexCode = readFile2Char((char*)vertexPath.c_str());
	fragmentCode = readFile2Char((char*)fragmentPath.c_str());	
	// Print shaders just for fun
	//cout << vertexCode << endl << fragmentCode;
	glShaderSource(vertexShader, 1, (const GLchar **) &vertexCode, NULL);  
	glShaderSource(fragmentShader, 1, (const GLchar **) &fragmentCode, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	// Create an empty program to which shaders can be attached
	*program = glCreateProgram();
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);

	glBindAttribLocation(*program, 0, "in_Position");
	glBindAttribLocation(*program, 1, "in_Color");

	// Link program. Each shader attached will become executalbes to run shader processor.
	glLinkProgram(*program);
	glUseProgram(*program);
}

void GLInit()
{
	// GL inits
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}
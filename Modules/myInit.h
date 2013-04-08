// myInit.h, Initiate all OpenGl things needed to get going
#ifndef MYINIT_H
#define MYINIT_H


#include <GL\glew.h>

#include "fixModel.h"

//extern GLfloat projectionMatrix[];

char* readFile2Char(char* file);
void shaderInit(GLuint* program);
void GLInit();


#endif
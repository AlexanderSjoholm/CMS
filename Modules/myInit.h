// myInit.h, Initiate all OpenGl things needed to get going
#ifndef MYINIT_H
#define MYINIT_H


#include <GL\glew.h>

#include "fixModel.h"
#include "myUtils.h"
#include "Model.h"
#include "loadTGA2.h"
#include "SolarSystem.h"
//extern GLfloat projectionMatrix[];

char* readFile2Char(char* file);
void programInit(std::map<std::string, GLuint>& shaderMap,
				 std::map<std::string, Model*>& modelMap,
				 std::map<std::string, GLuint>& textureMap,
				 std::map<std::string, GLuint>& normalMap);

void initPresetSystem(std::map<std::string, GLuint>& shaderMap,
					  std::map<std::string, Model*>& modelMap,
					  std::map<std::string, GLuint>& textureMap,
					  std::map<std::string, GLuint>& normalMap,
				      std::map<std::string, Object*>& presetMap);

void uploadShaders(std::map<std::string, GLuint>& shaderMap);
void uploadModels(std::map<std::string, Model*>& modelMap);
void uploadTextures(std::map<std::string, GLuint>& textureMap);
void uploadNormals(std::map<std::string, GLuint>& normalMap);

void shaderInit(GLuint* program, std::string vertexPath, std::string fragmentPath);
void GLInit();


#endif
#ifndef EDITOR_H
#define EDITOR_H

#include "SolarSystem.h"
#include "Object.h"
#include "Model.h"
#include "myUtils.h"

class Editor
{
public:
	Editor(std::map<std::string, Model*> modelMap,
		   std::map<std::string, GLuint> shaderMap,
		   std::map<std::string, GLuint> textureMap,
		   std::map<std::string, GLuint> normalMap,
		   std::map<std::string, Object*> presetMap);
	void edit(SolarSystem& solsystem, std::vector<bool>& states, Object* currentObject);
	void printHelp();
	void printWelcome();
	void deleteSolarSystem(SolarSystem& solsystem);
	Object* deleteObject(SolarSystem& solsystem, Object* _currentObject);

	void initDefaultSystem(SolarSystem& solsystem);

	void planetEditor(Object* currentObject);
	void setNewObject(Object* newObject, Object* currentObject);
	void Editor::planetCreator();

	void printTextures();
	void printShaders();
	void printNormals();
	void printPresets();

private:
	std::map<std::string, Model*> modelMap;
	std::map<std::string, GLuint> shaderMap;
	std::map<std::string, GLuint> textureMap;
	std::map<std::string, GLuint> normalMap;
	std::map<std::string, Object*> presetMap;
};

#endif
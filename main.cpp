#include "stdafx.h" // Gör att Kompilatorn hittar filen

#include "Modules/myInit.h"
#include "Modules/myUtils.h"
#include "Modules/GL_utilities.h"
#include "Modules/myLoadObj.h"
#include "Modules/LoadTGA2.h"
#include "Modules/Model.h"
#include "Modules/Object.h"
#include "Modules/Player.h"
#include "Modules/fixModel.h"
#include "Modules/SolarSystem.h"
#include "Modules/Physics.h"
#include "Modules/Editor.h"
#include <random>
#include <iostream>


//enum ProgramState;

// Reference to shader program
GLuint program, program2, program3, sunShader, earthShader, phongNoTex, phongTex;
std::map<std::string, Model*> modelMap;
std::map<std::string, GLuint> shaderMap;
std::map<std::string, GLuint> textureMap;
std::map<std::string, GLuint> normalMap;
std::map<std::string, Object*> presetMap;
std::vector<bool> states(10,false);

//std::vector<bool> states;


std::list<Object*> allObjects;

float dt = 0;
float accTime = 0;
//GLuint skyboxShader, skyboxTexture;


SolarSystem solsystem;
Physics physEngine;
std::string derp;
//Model* skyboxModel = new Model;


#

int main()
{
	
	// SFML window that will host our OpenGL magic
	
    sf::Window window(sf::VideoMode(1920, 1080), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	sf::View view;
	
	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2),  window);
	
	glewInit();
	
	Player	player;
	programInit(shaderMap, modelMap, textureMap, normalMap);
	initPresetSystem(shaderMap, modelMap, textureMap, normalMap, presetMap);
	Editor editor(modelMap, shaderMap, textureMap, normalMap, presetMap);

	GLInit();
	
	// ---------------------- MODELS -------------------------------
	Model		sphereModel, unitSquareModel;
	generateSphere(&sphereModel, 50);						sphereModel.upload();
	myLoadObj("Models/unitSquare.obj", &unitSquareModel);	unitSquareModel.upload();

	loadNoise();

	// Add terrain information to earth
	cv::Mat earthBumpMap = cv::imread("Textures/earthBumpMap.jpg");
	earthBumpMap.convertTo(earthBumpMap, CV_32F);
	int height = earthBumpMap.rows;
	int width = earthBumpMap.cols;
	std::cout << "Dims: " << height << ", " << width << std::endl;
	std::cout << "Dims: " << *earthBumpMap.row(height-1).col(5).ptr<float>() << std::endl;

	// ---------------------- OBJECTS -------------------------------
	// Initiation of all objects in the program
	// ShaderParameters = (ambientCoeff, diffuseCoeff, specularCoeff, specularExponent)
	cv::Vec4f standardShaderParameters(0.2f, 0.5f, 0.8f, 10);

	Object squareNormalMap, squareSpecularityMap;
	GLuint earthNormalMap, earthSpecularityMap, earthTextureDay, earthTextureNight;
	GLuint normalMapShader, specularityMapShader;

	shaderInit(&phongNoTex, "Shaders/phongNoTex.vert", "Shaders/phongNoTex.frag");
	shaderInit(&normalMapShader, "Shaders/normalMap.vert", "Shaders/normalMap.frag");
	shaderInit(&specularityMapShader, "Shaders/specularityMap.vert", "Shaders/specularityMap.frag");
	
	LoadTGATextureSimple("Textures/earthTextureDay.tga", &earthTextureDay);
	LoadTGATextureSimple("Textures/earthTextureNight.tga", &earthTextureNight);
	LoadTGATextureSimple("Textures/earthNormalMap.tga", &earthNormalMap);
	LoadTGATextureSimple("Textures/earthSpecularityMap.tga", &earthSpecularityMap);

	//squareNormalMap.init(&unitSquareModel, phongNoTex, standardShaderParameters, 0, 0, 0, earthNormalMap);
	squareNormalMap.init(&unitSquareModel, normalMapShader, standardShaderParameters, 0, 0, 0, earthNormalMap);
	squareSpecularityMap.init(&unitSquareModel, specularityMapShader, standardShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	squareNormalMap.set(cv::Vec3f(100,0,0),  cv::Vec3f(50,50,100), cv::Vec3f(0, pi/2, -pi/2), cv::Vec3f(0,0,0), 1);
	squareSpecularityMap.set(cv::Vec3f(100,0,0),  cv::Vec3f(50,50,100), cv::Vec3f(0, pi/2, -pi/2), cv::Vec3f(0,0,0), 1);
	

	// ---------------------- SKYSPHERE -------------------------------
	Object skysphere;
	GLuint skysphereTexture, skyboxShader;
	LoadTGATextureSimple("Textures/spaceBox6.tga", &skysphereTexture);


	shaderInit(&skyboxShader, "Shaders/skybox.vert", "Shaders/skybox.frag");

	skysphere.init(&sphereModel, skyboxShader, standardShaderParameters, skysphereTexture);
	skysphere.set(player.position,  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	


	int item;

	// SFML built-in clock
	sf::Clock clock;
	states[RUNNING] = true;
    states[EDITOR] = true;
	states[STARTUP] = true;
	/*
	bool running = true;
    bool runningEditor = true;
	bool startup = true;
	bool selectObject = false;
	bool cooldown = false;
	*/
	Object* currentObject = NULL;
	Object* playerObject = NULL;
	

	while (states[RUNNING])
    {
		dt = clock.getElapsedTime().asSeconds();
		if(states[EDITOR])
		{
			window.setVisible(false);
			if(states[SELECTOBJECT])
			{
				solsystem.getObjects(&allObjects);
				currentObject = getSelectedObject(&allObjects, &player);
				allObjects.clear();
			}
			editor.edit(solsystem, states, currentObject);
			states[STARTUP] = false;
			sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2),  window);
			clock.restart();
			window.setActive();

			states[SELECTOBJECT] = false;
			currentObject = NULL;
			
			states[EDITOR] = false;
			window.setVisible(true);
		}

		else
		{
			clock.restart();
			if (playerObject != NULL)
			{
				std::cout << player.position << std::endl;
			
				player.move(playerObject->position);
				//player.position = playerObject->position;

				std::cout << playerObject->position << std::endl;
			}
			
			handleEvents(&window, states, &item, playerObject, &player, dt);
			player.lookAtUpdate(dt);

			// Plocka ut all planeters positioner
			std::list<Object*> allObjects;
			solsystem.getObjects(&allObjects);
			std::vector<cv::Vec3f> positionVector;
			std::vector<cv::Vec3f> radiusVector;
			

			std::list<Object*>::iterator i = allObjects.begin();

			for (i++ ; i != allObjects.end(); ++i)
			{
				positionVector.push_back((*i)->position);
				radiusVector.push_back((*i)->scale);
				//std::cout << "Scale: " << (*i)->scale << std::endl;
			}

			int numberOfPlanets = positionVector.size();

			GLfloat* positions = makeArray(positionVector);
			GLfloat* radius = makeArray(radiusVector);
			

			/////////////////////////////////   SKYBOX   /////////////////////////////////////////
			window.setActive();
			//drawSkybox(&player, &skyboxModel, skyboxShader, skyboxTexture);

			glDisable(GL_DEPTH_TEST);
			//skybox.set(player.position,  cv::Vec3f(5,5,5), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
			//skybox.draw(&player);
			skysphere.set(player.position,  cv::Vec3f(5,5,5), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
			skysphere.draw(&player, 0, dt);
			glEnable(GL_DEPTH_TEST);

			/////////////////////////////////   ALX   /////////////////////////////////////////

			if (states[NORMALMAP])
			{
				squareNormalMap.draw(&player, dt, numberOfPlanets, positions, radius);
			}
			if (states[SPECULARITYMAP])
			{
				squareSpecularityMap.draw(&player, dt, numberOfPlanets, positions, radius);
			}

			/////////////////////////////////   ALX   /////////////////////////////////////////




		
			if(!states[COOLDOWN] && item == 1)
			{
				Object* newItem = presetMap["Earth"]->clone();
				newItem->set(player.position,  cv::Vec3f(0.25,0.25,0.25), cv::Vec3f(0,0,0), 10*player.getLookAtDirection(), 1);
				solsystem.addItem(newItem);
				std::cout << 10*normalize(player.position - player.lookAtVector) << std::endl;
				states[COOLDOWN] = true;
			}
			item = 0;
			if(states[ENABLEGRAVITY] && playerObject == NULL)
			{
				playerObject = presetMap["Earth"]->clone();
				playerObject->set(player.position,  cv::Vec3f(1.25,1.25,1.25), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
				solsystem.addPlayerItem(playerObject);
				states[ENABLEGRAVITY] = false;
			}
			if(states[DISABLEGRAVITY] && playerObject != NULL)
			{
				solsystem.removePlayerItem();
				playerObject = NULL;
				states[DISABLEGRAVITY] = false;
			}

			solsystem.update(physEngine, dt*0.5);
			solsystem.draw(&player, accTime);
			accTime += dt;

			window.display();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		cv::waitKey(0);
	}
    // release resources...
	glDeleteVertexArrays(1, &sphereModel.VAO);
	//glDeleteVertexArrays(1, &skyboxModel.VAO);
	//glDeleteVertexArrays(1, &unitSquareModel.VAO);
	//glDeleteVertexArrays(1, &groundModel.VAO);
	
	return 0;
}
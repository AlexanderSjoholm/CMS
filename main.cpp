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
#include <random>
#include <iostream>
// Reference to shader program
GLuint program, program2, program3, sunShader, earthShader, phongNoTex, phongTex;
float dt = 0;
SolarSystem solsystem;
Physics physEngine;

int main()
{
	// SFML window that will host our OpenGL magic
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	sf::View view;

	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2),  window);
	
	glewInit();

	// 
	GLuint planetShader, planetDeluxeShader;
	shaderInit(&planetShader, "Shaders/planet.vert", "Shaders/planet.frag");
	shaderInit(&planetDeluxeShader, "Shaders/planetDeluxe.vert", "Shaders/planetDeluxe.frag");
	shaderInit(&program, "Shaders/test.vert", "Shaders/test.frag");
	shaderInit(&sunShader, "Shaders/sun.vert", "Shaders/sun.frag");
	shaderInit(&earthShader, "Shaders/earth.vert", "Shaders/earth.frag");
	shaderInit(&earthShader, "Shaders/earth.vert", "Shaders/earth.frag");
	shaderInit(&phongNoTex, "Shaders/phongNoTex.vert", "Shaders/phongNoTex.frag");
	shaderInit(&phongTex, "Shaders/phongTex.vert", "Shaders/phongTex.frag");

	GLInit();

	Model		sphereModel, unitSquareModel, groundModel;
	Object		square, ground, sphere1, sphere2, sphere3;
	Player		player;

	// ---------------------- MODELS -------------------------------
	generateSphere(&sphereModel, 50);						sphereModel.upload();
	myLoadObj("Models/unitSquare.obj", &unitSquareModel);	unitSquareModel.upload();
	myLoadObj("Models/unitSquare50.obj", &groundModel);		groundModel.upload();
	
	
	// Load Textures
	GLuint testTex;
	
	
	
	LoadTGATextureSimple("Textures/testNormalMap.tga", &testTex);
	loadNoise();

	//TextureData grassTex;
	//LoadTGATexture("Textures/earthTexture2.tga", &grassTex);


	// Add terrain information to earth
	cv::Mat earthBumpMap = cv::imread("Textures/earthBumpMap.jpg");
	earthBumpMap.convertTo(earthBumpMap, CV_32F);
	int height = earthBumpMap.rows;
	int width = earthBumpMap.cols;
	std::cout << "Dims: " << height << ", " << width << std::endl;
	std::cout << "Dims: " << *earthBumpMap.row(height-1).col(5).ptr<float>() << std::endl;

	

	//bumpMySphere(&groundModel, &earthBumpMap);

	//moon.init(&sphereModel, program, standardShaderParameters, earthTextureDay, earthTextureNight);
		

	// ---------------------- OBJECTS -------------------------------
	// Initiation of all objects in the program
	// ShaderParameters = (ambientCoeff, diffuseCoeff, specularCoeff, specularExponent)
	cv::Vec4f standardShaderParameters(0.2f, 0.5f, 0.8f, 10);

	// Sun
	Object sun;
	GLuint sunTexture;
	LoadTGATextureSimple("Textures/sunTexture.tga", &sunTexture);
	sun.init(&sphereModel, sunShader, standardShaderParameters, sunTexture);
	sun.set(cv::Vec3f(0,0,0),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Mercury
	Object mercury;
	GLuint mercuryTexture;
	LoadTGATextureSimple("Textures/mercuryTexture.tga", &mercuryTexture);
	mercury.init(&sphereModel, planetShader, standardShaderParameters, mercuryTexture);
	mercury.set(cv::Vec3f(-10,0,0),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Venus
	Object venus;
	GLuint venusTexture;
	LoadTGATextureSimple("Textures/venusTexture.tga", &venusTexture);
	venus.init(&sphereModel, planetShader, standardShaderParameters, venusTexture);
	venus.set(cv::Vec3f(-8,0,2),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Earth
	Object earth;
	GLuint earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap;
	LoadTGATextureSimple("Textures/earthTextureDay.tga", &earthTextureDay);
	LoadTGATextureSimple("Textures/earthTextureNight.tga", &earthTextureNight);
	LoadTGATextureSimple("Textures/earthSpecularityMap.tga", &earthSpecularityMap);
	LoadTGATextureSimple("Textures/earthNormalMap.tga", &earthNormalMap);
	cv::Vec4f earthShaderParameters(0.2f, 0.5f, 0.5f, 3);
	earth.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	earth.set(cv::Vec3f(-10,0,-10),  cv::Vec3f(10,10,10), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Moon
	Object moon;
	GLuint moonTexture, moonNormalMap;
	LoadTGATextureSimple("Textures/moonTexture.tga", &moonTexture);
	LoadTGATextureSimple("Textures/moonNormalMap.tga", &moonNormalMap);
	moon.init(&sphereModel, planetDeluxeShader, earthShaderParameters, moonTexture, 0, 0, moonNormalMap);
	moon.set(cv::Vec3f(-7,0,-7),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Mars
	Object mars;
	GLuint marsTexture, marsNormalMap;
	LoadTGATextureSimple("Textures/marsTexture.tga", &marsTexture);
	LoadTGATextureSimple("Textures/marsNormalMap.tga", &marsNormalMap);
	mars.init(&sphereModel, planetShader, standardShaderParameters, marsTexture, 0, 0, marsNormalMap);
	mars.set(cv::Vec3f(-6,0,4),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Jupiter
	Object jupiter;
	GLuint jupiterTexture;
	LoadTGATextureSimple("Textures/jupiterTexture.tga", &jupiterTexture);
	jupiter.init(&sphereModel, planetShader, standardShaderParameters, jupiterTexture);
	jupiter.set(cv::Vec3f(-4,0,6),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));
	
	// Saturn
	Object saturn;
	GLuint saturnTexture;
	LoadTGATextureSimple("Textures/saturnTexture.tga", &saturnTexture);
	saturn.init(&sphereModel, planetShader, standardShaderParameters, saturnTexture);
	saturn.set(cv::Vec3f(-2,0,8),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Uranus
	Object uranus;
	GLuint uranusTexture;
	LoadTGATextureSimple("Textures/uranusTexture.tga", &uranusTexture);
	uranus.init(&sphereModel, planetShader, standardShaderParameters, uranusTexture);
	uranus.set(cv::Vec3f(0,0,10),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Neptune
	Object neptune;
	GLuint neptuneTexture;
	LoadTGATextureSimple("Textures/neptuneTexture.tga", &neptuneTexture);
	neptune.init(&sphereModel, planetShader, standardShaderParameters, neptuneTexture);
	neptune.set(cv::Vec3f(2,0,8),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	// Pluto
	Object pluto;
	GLuint plutoTexture;
	LoadTGATextureSimple("Textures/plutoTexture.tga", &plutoTexture);
	pluto.init(&sphereModel, planetShader, standardShaderParameters, plutoTexture);
	pluto.set(cv::Vec3f(4,0,6),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));


	
	ground.init(&groundModel, phongTex, standardShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, testTex);
	ground.set(cv::Vec3f(0,-1,0),  cv::Vec3f(10,10,10), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));

	sphere1.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	sphere1.set(cv::Vec3f(5,0,0),  cv::Vec3f(0.1,0.1,0.1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));
	
	sphere2.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	sphere2.set(cv::Vec3f(0,5,0),  cv::Vec3f(0.3,0.3,0.3), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));
	
	sphere3.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	sphere3.set(cv::Vec3f(0,0,5),  cv::Vec3f(0.9,0.9,0.9), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));
	

	//square.set(cv::Vec3f(0,0,-40), cv::Vec3f(30,30,30), cv::Vec3f(1.5,0,0), cv::Vec3f(0,0,0));
	//square.update(cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), cv::Vec3f(-0.05f,0,0));
	//earth.addSatellite(&moon, 3);
	//sun.addSatellite(&earth, 10);
	//solsystem.addStar(&sun, true, cv::Vec3f(1,1,1));
	//solsystem.addStar(&earth, true, cv::Vec3f(1,1,1));

	// Uplaod models
	
	

	int item;

	// SFML built-in clock
	sf::Clock clock;
	bool running = true;
    while (running)
    {
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		handleEvents(&window, &running, &item, &player, dt);
		player.lookAtUpdate(dt);

		earth.update(cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), cv::Vec3f(0,-0.01f,0));
		moon.update(cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), cv::Vec3f(0,0.02f,0));

		window.setActive();

		if(item == 1)
		{
			Object* newItem = new Object();
			newItem->init(&sphereModel, program, standardShaderParameters, earthTextureDay, earthTextureNight);
			newItem->set(player.position,  cv::Vec3f(0.25,0.25,0.25), cv::Vec3f(0,0,0), 10*normalize(player.lookAtVector - player.position));
			solsystem.addItem(newItem);
			std::cout << 10*normalize(player.position - player.lookAtVector) << std::endl;
		}
		item = 0;
		
		//solsystem.update(physEngine, dt);
		//solsystem.draw(&player);

		//square.draw(&player);
		
		ground.draw(&player);

		sun.draw(&player);
		mercury.draw(&player);
		venus.draw(&player);
		earth.draw(&player);
		moon.draw(&player);
		mars.draw(&player);
		jupiter.draw(&player);
		saturn.draw(&player);
		uranus.draw(&player);
		neptune.draw(&player);
		pluto.draw(&player);

		sphere1.draw(&player);
		sphere2.draw(&player);
		sphere3.draw(&player);

        window.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // release resources...
	glDeleteVertexArrays(1, &sphereModel.VAO);
	glDeleteVertexArrays(1, &unitSquareModel.VAO);
	glDeleteVertexArrays(1, &groundModel.VAO);
	
    return 0;
}


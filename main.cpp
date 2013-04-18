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
#include <iostream>
// Reference to shader program
GLuint program, program2, program3, sunProgram;
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
	shaderInit(&program, "Shaders/test.vert", "Shaders/test.frag");
	shaderInit(&sunProgram, "Shaders/sun.vert", "Shaders/sun.frag");
	GLInit();
	
	//Model		cubeModel, sphereModel, bladeModel, testModel;
	//Object		cube, sphere, blade;

	Model		sphereModel;
	Object		earth, sun, moon;
	Player		player;

	//myLoadObj("Models/myUnitCube.obj", &cubeModel);

	GLuint earthTextureDay, earthTextureNight;
	LoadTGATextureSimple("Textures/earthTextureDay.tga", &earthTextureDay);
	LoadTGATextureSimple("Textures/earthTextureNight.tga", &earthTextureNight);

	//cv::Mat earthBumpMap = cv::imread("Textures/earthBumpMap.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//int height = earthBumpMap.rows;
	//int width = earthBumpMap.cols;
	//std::cout << "Dims: " << height << ", " << width << std::endl;
	//cube.init(&cubeModel, program, "inPosition", "inNormal", "inTexCoord", earthTextureDay);
	//cubeModel.upload();

	generateSphere(&sphereModel, 50);

	//bumpMySphere(&sphereModel, &earthBumpMap);
	sphereModel.upload();
	moon.init(&sphereModel, program, "inPosition", "inNormal", "inTexCoord", earthTextureDay, earthTextureNight);
	earth.init(&sphereModel, program, "inPosition", "inNormal", "inTexCoord", earthTextureDay, earthTextureNight);
	sun.init(&sphereModel, sunProgram, "inPosition", "inNormal", "inTexCoord");
	//sphereModel.upload();


	sun.set(cv::Vec3f(0,0,0), cv::Vec3f(2,2,2), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));
	earth.set(cv::Vec3f(10,0,0),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0));
	moon.set(cv::Vec3f(14,0,0), cv::Vec3f(0.5,0.5,0.5), cv::Vec3f(0,0,0), cv::Vec3f(0,-10,0));
	
	
	earth.addSatellite(&moon, 3);
	sun.addSatellite(&earth, 10);
	solsystem.addStar(&sun, true, cv::Vec3f(1,1,1));

	// SFML built-in clock
	sf::Clock clock;
	bool running = true;
    while (running)
    {
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		handleEvents(&window, &running, &player, dt);
		player.lookAtUpdate(dt);

		
		earth.update(cv::Vec3f(0,0,0),  cv::Vec3f(0,0,0), cv::Vec3f(0,-0.05f,0));
		
		//cube.update(0,0,0, 0,0,0, 0,0.04f,0);

		window.setActive();

		

		//cube.draw(&player);
		solsystem.update(physEngine, dt);
		solsystem.draw(&player);

		//solsystem.draw(player);
		//earth.draw(&player);
        window.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//std::cout << "dt: " << dt << std::endl;
		//std::cin >> asdf;

    }

    // release resources...
	//glDeleteVertexArrays(1, &cubeModel.VAO);
	glDeleteVertexArrays(1, &sphereModel.VAO);
	
    return 0;
}


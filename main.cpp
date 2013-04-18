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
GLuint program, program2, program3;
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
	GLInit();
	
	//Model		cubeModel, sphereModel, bladeModel, testModel;
	//Object		cube, sphere, blade;

	Model		sphereModel;
	Object		sphere;
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
	sphere.init(&sphereModel, program, "inPosition", "inNormal", "inTexCoord", earthTextureDay, earthTextureNight);
	//sphereModel.upload();

	//glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, earthTexture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glUniform1i(glGetUniformLocation(program, "Tex0"), 0);

	//cube.update(0,0,0, 20,20,20, 0,0,0);
	//sphere.update(0,0,0, 5,5,5, 0,0,0);
	
	//cube.update(cv::Vec3f(0,0,0),  cv::Vec3f(20,20,20), cv::Vec3f(0,0,0));
	sphere.update(cv::Vec3f(0,0,0),  cv::Vec3f(5,5,5), cv::Vec3f(0,0,0));

	//shaderInit(&program, "Shaders/test.vert", "Shaders/test.frag");
	//shaderInit(&program2, "Shaders/test.vert", "Shaders/phongShaderNoTexture.frag");
	/*
	myLoadObj("Models/unitSphere.obj", &testModel);
	sphere.init(&testModel, program, "in_Position", "in_Normal", "Tex0", );
	sphere.set(cv::Vec3f(0,0,0), cv::Vec3f(2,2,2), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));
	testModel.upload();

	myLoadObj("Models/unitSphere.obj", &bladeModel);
	blade.init(&bladeModel, program2,  "in_Position", "in_Normal");
	blade.set(cv::Vec3f(10,0,0), cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0));
	//blade.setOrbit(&sphere, 10);
	bladeModel.upload();

	myLoadObj("Models/unitSphere.obj", &cubeModel);
	cube.init(&cubeModel, program, "in_Position", "in_Normal");
	cube.set(cv::Vec3f(13,0,0), cv::Vec3f(0.5,0.5,0.5), cv::Vec3f(0,0,0), cv::Vec3f(0,-10,0));
	//cube.setOrbit(&blade, 3);
	cubeModel.upload();
	/*
	cubeModel.init(	
				program, 
				"in_Position",
				"in_Color");
	cube.init(&cubeModel, program, "in_Position", "in_Normal");
	LoadTGATextureSimple("Textures/maskros512.tga", &texture);
	*/
	/*
	blade.addSatellite(&cube, 3);
	sphere.addSatellite(&blade, 10);	
	solsystem.addStar(&sphere);
	//std::cout << cube.orbits->position << std::endl;

	solsystem.addPlanet(&cube);
	solsystem.addPlanet(&blade);
	solsystem.addPlanet(&sphere);

	std::string asdf;
	*/

	// SFML built-in clock
	sf::Clock clock;
	bool running = true;
    while (running)
    {
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		handleEvents(&window, &running, &player, dt);
		player.lookAtUpdate(dt);

		
		sphere.update(cv::Vec3f(0,0,0),  cv::Vec3f(0,0,0), cv::Vec3f(0,0.01f,0));
		
		//cube.update(0,0,0, 0,0,0, 0,0.04f,0);
		//solsystem.update(physEngine, dt);

		window.setActive();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//cube.draw(&player);

		sphere.draw(&player);

		//solsystem.draw(player);
		//sphere.draw(&player);
        window.display();
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//std::cout << "dt: " << dt << std::endl;
		//std::cin >> asdf;

    }

    // release resources...
	//glDeleteVertexArrays(1, &cubeModel.VAO);
	glDeleteVertexArrays(1, &sphereModel.VAO);
	
    return 0;
}


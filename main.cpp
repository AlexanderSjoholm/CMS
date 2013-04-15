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

// Reference to shader program
GLuint program, program2, program3;
float dt;
SolarSystem solsystem;
Physics physEngine;

int main()
{

	
	// SFML window that will host our OpenGL magic
    sf::Window window(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	
	Model	cubeModel, bladeModel, testModel;
	Object		cube, sphere, blade;
	Player		player;
	GLuint		texture;
	
	Model* skyboxModel;
	/*
	skyboxModel = LoadModelPlus("Models/skybox.obj",
										program,
										"in_Position",
										"in_Normal",
										"inTexCoord");
										*/
	glewInit();
	GLInit();

	shaderInit(&program, "Shaders/test.vert", "Shaders/test.frag");
	shaderInit(&program2, "Shaders/test.vert", "Shaders/phongShaderNoTexture.frag");

	myLoadObj("Models/unitSphere.obj", &testModel);
	sphere.init(&testModel, program, "in_Position", "in_Normal");
	sphere.set(cv::Vec3f(0,0,0), cv::Vec3f(2,2,2), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0));
	testModel.upload();

	myLoadObj("Models/unitSphere.obj", &bladeModel);
	blade.init(&bladeModel, program2,  "in_Position", "in_Normal");
	blade.set(cv::Vec3f(10,0,0), cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0));
	blade.setOrbit(&sphere, 10);
	bladeModel.upload();

	myLoadObj("Models/unitSphere.obj", &cubeModel);
	cube.init(&cubeModel, program, "in_Position", "in_Normal");
	cube.set(cv::Vec3f(13,0,0), cv::Vec3f(0.5,0.5,0.5), cv::Vec3f(0,0,0), cv::Vec3f(0,-10,0));
	cube.setOrbit(&blade, 3);
	cubeModel.upload();
	/*
	cubeModel.init(	
				program, 
				"in_Position",
				"in_Color");
	cube.init(&cubeModel, program, "in_Position", "in_Normal");
	LoadTGATextureSimple("Textures/maskros512.tga", &texture);
	*/
	solsystem.addPlanet(&cube);
	solsystem.addPlanet(&blade);	
	solsystem.addPlanet(&sphere);


	// SFML built-in clock
	sf::Clock clock;
	bool running = true;
    while (running)
    {
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		handleEvents(&window, &running, &player, dt);
		player.lookAtUpdate(dt);
		
		//cube.update(0,0,0, 0,0,0, 0,0.04f,0);
		solsystem.update(physEngine, dt);
		window.setActive();

		solsystem.draw(player);
		//sphere.draw(&player);
		std::cout << cube.position - blade.position << std::endl;
        window.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

    // release resources...
	glDeleteVertexArrays(1, &cubeModel.VAO);
	
    return 0;
}


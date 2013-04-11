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

// Reference to shader program
GLuint program, program2;
float dt;
SolarSystem solsystem;

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

	//shaderInit(&program); Är konstigt, antagligen jag som är retard.
	shaderInit(&program, "Shaders/test.vert", "Shaders/test.frag");
	shaderInit(&program2, "Shaders/test.vert", "Shaders/phongShaderNoTexture.frag");

	myLoadObj("Models/unitSphere.obj", &testModel);
	sphere.init(&testModel, program, "in_Position", "in_Normal");
	testModel.upload();

	myLoadObj("Models/windmill-blade.obj", &bladeModel);
	blade.init(&bladeModel, program2,  "in_Position", "in_Normal");
	bladeModel.upload();
	blade.set(1,1,1, 1,1,1, 0,0.04f,0);

	solsystem.addPlanet(&blade);	
	solsystem.addPlanet(&sphere);
		
	/*
	cubeModel.init(	
				program, 
				"in_Position",
				"in_Color");
	cube.init(&cubeModel, program, "in_Position", "in_Normal");
	LoadTGATextureSimple("Textures/maskros512.tga", &texture);
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
		
		//cube.update(0,0,0, 0,0,0, 0,0.04f,0);
		solsystem.update();
		window.setActive();

		// får av någon anledning inte rita ut mer än ett objekt i taget..
		solsystem.draw(player);
		//sphere.draw(&player);
	
        window.display();
    }

    // release resources...
	glDeleteVertexArrays(1, &cubeModel.VAO);
	
    return 0;
}


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

// Reference to shader program
GLuint program;
float dt;

int main()
{

	
	// SFML window that will host our OpenGL magic
    sf::Window window(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);
	
	Model	cubeModel;
	Object		cube;
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
	shaderInit(&program);
	GLInit();
	
	Model	testModel;
	myLoadObj("Models/unitSphere.obj", &testModel);

	cube.init(&testModel, program, "in_Position", "in_Normal");
	testModel.upload();
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
		cube.update(0,0,0, 0,0,0, 0,0.04f,0);

		window.setActive();

		cube.draw(&player);
	
        window.display();
    }

    // release resources...
	glDeleteVertexArrays(1, &cubeModel.VAO);
	
    return 0;
}


#include "stdafx.h"

#include "Modules/myInit.h"
#include "Modules/myUtils.h"
#include "Modules/GL_utilities.h"
#include "Modules/LoadTGA2.h"
#include "Modules/loadobj.h"
#include "Modules/Model.h"
#include "Modules/Object.h"
#include "Modules/Player.h"
#include "Modules/fixModel.h"

// Reference to shader program
GLuint program;

int main()
{
	// SFML window that will host our OpenGL magic
    sf::Window window(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
	
	my::Model	cubeModel;
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
	
	cubeModel.init(	
				program, 
				"in_Position",
				"in_Color");
	cube.init(&cubeModel);
	LoadTGATextureSimple("Textures/maskros512.tga", &texture);
	
    // Main loop presented by SFML,
	bool running = true;
    while (running)
    {
		handleEvents(&window, &running, &player);
		player.lookAtUpdate();
		cube.update(0,0,0, 0,0,0, 0,0.04f,0);

		window.setActive();

		cube.draw(&player);
	
        window.display();
    }

    // release resources...
	glDeleteVertexArrays(1, &cubeModel.VAO);
	
    return 0;
}


#include "stdafx.h"

#include "myInit.h"
#include "myUtils.h"
#include "GL_utilities.h"
#include "LoadTGA2.h"
#include "loadobj.h"
#include "Model.h"
#include "Object.h"
#include "Player.h"
#include "fixModel.h"

// Reference to shader program
GLuint program;

int main()
{
	// SFML window that will host our OpenGL magic
    sf::Window window(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

	my::Model	cubeModel;
	Object	cube;
	Player	player;
	GLuint	texture;
	
	
	Model* skyboxModel;

	std::cout << "Main 1" << std::endl;

	skyboxModel = LoadModelPlus("Models/skybox.obj",
										program,
										"in_Position",
										"in_Normal",
										"inTexCoord");

	glewInit();
	shaderInit(&program);
	GLInit();
	
	cubeModel.init(	
				program, 
				"in_Position",
				"in_Color");
	cube.init(&cubeModel);
	LoadTGATextureSimple("Models/maskros512.tga", &texture);

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


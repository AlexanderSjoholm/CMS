// myUtils.cpp

#include "stdafx.h"
#include "myUtils.h"

void handleEvents(sf::Window* window, bool* running, Player* player)
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == (sf::Event::Closed))
        {
            // end the program
            *running = false;
        }
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
        }
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
				*running = false;

			if (event.key.code == sf::Keyboard::F11)
				window->create(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
		}
    }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		std::cout << "Shit" << std::endl;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		player->moveForward();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		player->moveBackward();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		player->moveLeft();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->moveRight();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		player->lookUp();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		player->lookDown();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		player->lookLeft();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		player->lookRight();
}


// The rotation matrices will be changed for animation
// Perspective
#define nearFrustum 1.0
#define farFrustum 30.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projectionMatrix[] = {	2.0f*nearFrustum/(right-left), 0.0f,					(right+left)/(right-left),	0.0f,
								0.0f,					2.0f*nearFrustum/(top-bottom),	(top+bottom)/(top-bottom),	0.0f,
								0.0f,					0.0f,					-((float)farFrustum + (float)nearFrustum)/((float)farFrustum - (float)nearFrustum),	-2.0f*(float)farFrustum*(float)nearFrustum/((float)farFrustum - (float)nearFrustum),
								0.0f,					0.0f,					-1.0f,						0.0f };


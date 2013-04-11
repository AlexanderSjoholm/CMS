// myUtils.cpp

#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include "myUtils.h"

void handleEvents(sf::Window* window, bool* running, Player* player, float dt)
{
    sf::Event event;
	//relative mouse movements (brought to you by OpenCV because SFML sucks)
	cv::Vec2i dMouse;
	cv::Vec2i windowCenter;
	//sf::Vector2u windowCenter;
	int dx, dy;


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
			{
				window->close();
				window->create(sf::VideoMode::getDesktopMode(), "OpenGL", sf::Style::Fullscreen, sf::ContextSettings(32));
			}
		}
    }

	dx = sf::Mouse::getPosition(*window).x - window->getSize().x / 2;
	dy = sf::Mouse::getPosition(*window).y - window->getSize().y / 2; 
	dy = -dy; // Invert y axis
	
	//dMouse = sf::Mouse::getPosition(*window) - windowCenter;
	sf::Mouse::setPosition(sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2),  *window);


	if (dx != 0)
		player->xLook(dt, dx);
	if (dy != 0)
		player->yLook(dt, dy);


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		player->moveUp(dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		player->moveDown(dt);
		//std::cout << "Shit" << std::endl;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		player->moveForward(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		player->moveBackward(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		player->moveLeft(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		player->moveRight(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		player->lookUp(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		player->lookDown(dt);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		player->lookLeft(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		player->lookRight(dt);
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


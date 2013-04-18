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
#define farFrustum 100.0
#define right 1.0
#define left -1.0
#define top 1.0
#define bottom -1.0
GLfloat projectionMatrix[] = {	2.0f*nearFrustum/(right-left), 0.0f,					(right+left)/(right-left),	0.0f,
								0.0f,					2.0f*nearFrustum/(top-bottom),	(top+bottom)/(top-bottom),	0.0f,
								0.0f,					0.0f,					-((float)farFrustum + (float)nearFrustum)/((float)farFrustum - (float)nearFrustum),	-2.0f*(float)farFrustum*(float)nearFrustum/((float)farFrustum - (float)nearFrustum),
								0.0f,					0.0f,					-1.0f,						0.0f };

void generateSphere(Model* model, int subdivisions)
{
	// Creat a unit sphere with number of subdivisions

	// Auxilary varialbes
	cv::Vec3f vertex;
	cv::Vec2f texCoord;
	cv::Vec3i face;
	double phi = 0, theta = 0;

	//Wrap it up with the top vertex
	vertex[0] = 0;
	vertex[1] = 1;
	vertex[2] = 0;
	model->vertexArray.push_back(vertex);
	model->normalArray.push_back(vertex);
	texCoord[0] = 0.5;
	texCoord[1] = 0.975; // TexCoords larger than 0.975 ends up outside texture
	model->texCoordArray.push_back(texCoord);
	
	// Place vertices on sphere
	for (int i = 1; i < subdivisions - 1; i++)
	{
		// <= to get double vertives at seam 
		for (int j = 0; j <= subdivisions; j++)
		{
			// phi is angle around the y axis
			phi = j*2*pi/subdivisions;
			// theta is angle to the y axis
			theta = i*pi/subdivisions;

			vertex[0] = (float)(sin(theta)*cos(phi));
			vertex[1] = (float)cos(theta);
			vertex[2] = (float)(sin(theta)*sin(phi));

			model->vertexArray.push_back(vertex);
			// Due to the radius being one the normal is equal to the coresponding vertex
			model->normalArray.push_back(vertex);
			
			// Texture coordinates aref ound through phi and theta
			texCoord[0] = (1 - (float)j/subdivisions) * 0.975 + 0.001;
			texCoord[1] = (1 - (float)i/subdivisions) * 0.99;

			model->texCoordArray.push_back(texCoord);
		}
	}

	//Begin with the bottom vertex
	vertex[0] = 0;
	vertex[1] = -1;
	vertex[2] = 0;
	model->vertexArray.push_back(vertex);
	model->normalArray.push_back(vertex);
	texCoord[0] = 0.5;
	texCoord[1] = 0;
	model->texCoordArray.push_back(texCoord);

	// Now all vertices are placed in space
	// Combine these into Faces

	// Start with north pole
	for (int i = 0; i < subdivisions; i++)
	{
		face[0] = 0;
		face[1] = i + 1;
		face[2] = i + 2;
		model->indexArray.push_back(face);
		model->numberOfIndices += 3;
	}

	// offset is the number of indexes needed to acces the next vertex-row in the vertex vector;
	int offset = subdivisions + 1;

	// -3 to stop in before the south pole vertices get in the picture
	for (int i = 0; i < subdivisions - 3; i++)
	{
		for (int j = 0; j < subdivisions; j++)
		{
			// Face 1 - upper-left triangle
			face[0] = i*(offset) + 1 + j;
			face[1] = i*(offset) + 1 + offset + j;
			face[2] = i*(offset) + 1 + offset + j + 1;
			model->indexArray.push_back(face);
			model->numberOfIndices += 3;
			
			// Face 2 - lower-right triangle
			face[0] = i*(offset) + 1 + j;
			face[1] = i*(offset) + 1 + j + 1;
			face[2] = i*(offset) + 1 + offset + j + 1;
			model->indexArray.push_back(face);
			model->numberOfIndices += 3;
		}
	}

	// Finish with south pole
	int poleVertex = model->vertexArray.size() - 1;
	for (int i = 0; i < subdivisions; i++)
	{
		face[0] = poleVertex;
		face[1] = poleVertex - i - 1;
		face[2] = poleVertex - i - 2;
		model->indexArray.push_back(face);
		model->numberOfIndices += 3;
	}
}

void bumpMySphere(Model* model, cv::Mat* bumpMap)
{
	int height = bumpMap->rows;
	int width = bumpMap->cols;
	std::cout << "Dims: " << height << ", " << width << std::endl;

	for (int i = 0; i < model->vertexArray.size(); i++)
	{
		float u = model->texCoordArray[i][0];
		std::cout << "u: " << u << std::endl;
		float v = model->texCoordArray[i][1];
		std::cout << "v: " << v << std::endl;
		int row = floor(v*(height - 1));
		int col = floor(u*(width - 1));
		std::cout << "coords: " << row << ", " << col << std::endl;
		int scale = *bumpMap->row(row).col(col).ptr<int>();
		std::cout << "scale: " << scale << std::endl;
		//model->vertexArray[i]*scale/200;
	}
}
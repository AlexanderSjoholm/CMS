// myUtils.cpp

#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include "myUtils.h"
#include "SolarSystem.h"
#include "LoadTGA2.h"




void handleEvents(sf::Window* window, std::vector<bool>& states, int* item, Object* playerObject, Player* player, float dt)
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
            states[RUNNING] = false;
        }
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
        }
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
				states[RUNNING] = false;

			if (event.key.code == sf::Keyboard::E)
				states[EDITOR] = true;

			if (event.key.code == sf::Keyboard::G && !playerObject)
				states[ENABLEGRAVITY] = true;

			if (event.key.code == sf::Keyboard::F && playerObject)
				states[DISABLEGRAVITY] = true;

			if (event.key.code == sf::Keyboard::F11)
			{
				window->close();
				window->create(sf::VideoMode::getDesktopMode(), "OpenGL", sf::Style::Fullscreen, sf::ContextSettings(32));
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased)
				{
					states[COOLDOWN] = false;
				}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			*item = 1;
			std::cout << "leftbuttonreleased" << std::endl;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			states[EDITOR] = true;
			states[SELECTOBJECT] = true;
			std::cout << "right button pressed" << std::endl;
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
#define nearFrustum 2.0
#define farFrustum 500.0
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
	//texCoord[1] = 0.975; // TexCoords larger than 0.975 ends up outside texture
	texCoord[1] = 1.0; // TexCoords larger than 0.975 ends up outside texture
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
			//texCoord[0] = (1 - (float)j/subdivisions) * 0.975 + 0.001;
			//texCoord[1] = (1 - (float)i/subdivisions) * 0.99;

			texCoord[0] = (1 - (float)j/subdivisions);
			texCoord[1] = (1 - (float)i/subdivisions);

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
		float scale = *bumpMap->row(row).col(col).ptr<float>();
		std::cout << "scale: " << scale << std::endl;
		std::cout << "Before: " << model->vertexArray[i] << std::endl;
		model->vertexArray[i] = model->vertexArray[i] * (1 + scale/(255 * 5));
		std::cout << "After: " << model->vertexArray[i] << std::endl;
		//Sleep(2000);
	}
}

void drawSkybox(Player* player, Model* model, GLuint program, GLuint texture)
{
	glDisable(GL_DEPTH_TEST);
	glUseProgram(program);

	// Upload Transformations
	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMatrix"), 1, GL_TRUE, player->lookAtMatrix.ptr<GLfloat>());
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotX"), 1, GL_TRUE, rotX.ptr<GLfloat>());
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotY"), 1, GL_TRUE, rotY.ptr<GLfloat>());
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotZ"), 1, GL_TRUE, rotZ.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	
	player->lookAtUpload(program);
	// Upload shading parameters
	//glUniform1fv(glGetUniformLocation(program, "ambientCoeff"), 1, &ambientCoeff);
    //glUniform1fv(glGetUniformLocation(program, "diffuseCoeff"), 1, &diffuseCoeff);
    //glUniform1fv(glGetUniformLocation(program, "specularCoeff"), 1, &specularCoeff);
    //glUniform1uiv(glGetUniformLocation(program, "specularExponent"), 1, &specularExponent);
	
	glUniform3fv(glGetUniformLocation(program, "inColor"), 1, cv::Mat(cv::Vec3f(1,1,1)).ptr<GLfloat>());
	
	
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotZ"), 1, GL_TRUE, rotZ.ptr<GLfloat>());

	

	// Bind the right textures
	if (texture != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(program, "Tex0"), 0);
	}

	// Bind the model's VAO and buffers to the program specified by the object
	glBindVertexArray(model->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

	glBindBuffer(GL_ARRAY_BUFFER, model->NBO);
	glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));

	glBindBuffer(GL_ARRAY_BUFFER, model->TBO);
	glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	
	glDrawElements(GL_TRIANGLES, model->numberOfIndices, GL_UNSIGNED_INT, 0);
	glEnable(GL_DEPTH_TEST);
}


GLuint noiseTexture;
std::normal_distribution<double> normalDistribution(0, 1);
std::mt19937 generator;	
void loadNoise()
{
	LoadTGATextureSimple("Textures/noise.tga", &noiseTexture);
}

Object* getSelectedObject(std::list<Object*>* allObjects, Player* player)
{
	if(!allObjects || allObjects->empty())
		return NULL;
	float a,b,c,d,e,f; 
	float minDistance = 999999999999;
	Object* closestObject;
	cv::Vec3f objectPosition;
	cv::Vec4f distanceVector;
	cv::Vec4f playerPosition(player->position[0], player->position[1], player->position[2], 1);
	cv::Vec4f lookAt(player->lookAtVector[0], player->lookAtVector[1], player->lookAtVector[2], 1);
	cv::Matx<float,4,4> plueckerCoords = playerPosition*lookAt.t() - lookAt*playerPosition.t();
	a = plueckerCoords(0,1);
	b = plueckerCoords(0,2);
	c = plueckerCoords(0,3);
	d = plueckerCoords(1,2);
	e = plueckerCoords(1,3);
	f = plueckerCoords(2,3);

	cv::Matx<float,4,4> dualPlueckerCoords(0, f, -e, d,
										   -f, 0, c, -b,
										   e, -c, 0, a,
										   -d, b, -a, 0);

	cv::Matx<float,3,3> A(0, f, -e,
						  -f, 0, c,
						  e, -c, 0);

	A = A.t()*A;

	dualPlueckerCoords = sqrt(2)/(A(0,0)*A(1,1)*A(2,2))*dualPlueckerCoords;
	
	
	for(std::list<Object*>::iterator it = allObjects->begin(); it != allObjects->end(); ++it)
	{
		objectPosition = (*it)->position;
		distanceVector = dualPlueckerCoords*cv::Vec4f(objectPosition(0), objectPosition(1), objectPosition(2), 1);
		std::cout << "vectorNorm" << cv::norm(cv::Vec3f(distanceVector(0),distanceVector(1),distanceVector(2))) << std::endl;
		if (minDistance > cv::norm(cv::Vec3f(distanceVector(0),distanceVector(1),distanceVector(2))))
		{
			minDistance = cv::norm(cv::Vec3f(distanceVector(0),distanceVector(1),distanceVector(2)));
			closestObject = (*it);
		}

	}
	std::cout << "plueckerCoords: " << plueckerCoords << std::endl;
	std::cout << "dualPlueckerCoords: " << dualPlueckerCoords << std::endl;
	std::cout << "plueckerCoords: " << a << "  " << b << "  " << c << "  " << d << "  " << e << "  " << f << std::endl;
	std::cout << "Minimum distance " << minDistance << std::endl;
	
	return closestObject;

	
	return NULL;
		
}
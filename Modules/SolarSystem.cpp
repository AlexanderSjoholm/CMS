//SolarSystem.cpp

#include "stdafx.h"
//#include "../../Repository/stdafx.h" // G�r att editorn hittar filen (suck)

#include "SolarSystem.h"

void SolarSystem::addPlanet(Object * planet, bool isLightSource, cv::Vec3b color)
{
	planetList.push_front(*planet);
	
	//Hax, kommer f�lja med planeten hela v�gen tack vare OpenCVs matrisimplmentation
	if (isLightSource)
	{
		Light tempLight;
		tempLight.position = planet->position;
		tempLight.color = color;
		lightList.push_front(&tempLight);
	}
	return;
}

void SolarSystem::update()
{
	//Call physics blahblahblah

	//Since no physics:
	for (std::list<Object>::iterator it = planetList.begin(); it != planetList.end(); it++)
	{
		it->update(0,0,0, 0,0,0, 0,0.04f,0);
	}
	return;
}

void SolarSystem::draw(Player & player)
{
	//Draw everything
	int herp = 0;
	for (std::list<Object>::iterator it = planetList.begin(); it != planetList.end(); it++)
	{
		herp++;
		it->draw(&player);
		//std::cout << "drawing stuff;  " << herp << std::endl;
	}
	return;
}

//TODO
void SolarSystem::addItem(Object * item)
{
	return;
}
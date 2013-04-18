//SolarSystem.cpp

#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)

#include "SolarSystem.h"

void SolarSystem::addPlanet(Object * planet, bool isLightSource, cv::Vec3b color)
{
	planetList.push_front(planet);
	/*
	//Hax, kommer följa med planeten hela vägen tack vare OpenCVs matrisimplmentation
	if (isLightSource)
	{
		Light tempLight;
		tempLight.position = &planet->position;
		tempLight.color = color;
		lightList.push_front(tempLight);
	}*/
	return;
}

void SolarSystem::addStar(Object * planet, bool isLightSource, cv::Vec3b color)
{
	starList.push_front(planet);
	
	//Hax, kommer följa med planeten hela vägen tack vare OpenCVs matrisimplmentation
	if (isLightSource)
	{
		Light tempLight;
		tempLight.position = &planet->position;
		tempLight.color = color;
		lightList.push_front(tempLight);
	}
	return;
}

void SolarSystem::update(Physics & PhysEngine, float dt)
{
	PhysEngine.updatePositions(starList, itemList, dt);
	return;
}

void SolarSystem::draw(Player & player)
{
	//Draw everything
	int herp = 0;
	for (std::list<Object*>::iterator it = planetList.begin(); it != planetList.end(); it++)
	{
		herp++;
		(*it)->draw(&player);
		//std::cout << "drawing stuff;  " << herp << std::endl;
	}
	return;
}

//TODO
void SolarSystem::addItem(Object * item)
{
	return;
}
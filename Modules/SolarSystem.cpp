//SolarSystem.cpp

#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)

#include "SolarSystem.h"


void SolarSystem::addStar(Object * star, bool isLightSource, cv::Vec3f color)
{
	starList.push_front(star);
	
	//Hax, kommer följa med planeten hela vägen tack vare OpenCVs matrisimplmentation
	if (isLightSource)
	{
		Light tempLight;
		tempLight.position = &star->position;
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

void SolarSystem::draw(Player * player)
{
	//Draw everything
	int herp = 0;
	for (std::list<Object*>::iterator it = starList.begin(); it != starList.end(); it++)
	{
		herp++;
		(*it)->draw(player);
	}
	return;
}


//TODO
void SolarSystem::addItem(Object * item)
{
	return;
}
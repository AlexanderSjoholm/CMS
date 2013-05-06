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

void SolarSystem::getObjects(std::list<Object*>* objectList)
{
	if(starList.empty())
		return;
	for(std::list<Object*>::iterator it = starList.begin(); it != starList.end(); it++)
	{
		(*it)->getSatellites(objectList);
	}
	return;
}

void SolarSystem::draw(Player * player, float dt)
{
	// Get all objects (needed for ray-casting)
	std::list<Object*> allObjects;
	getObjects(&allObjects);
	
	//Draw everything
	if (starList.empty())
		return;
	for (std::list<Object*>::iterator it = starList.begin(); it != starList.end(); it++)
	{
		(*it)->draw(player, &allObjects, dt);
	}
	for (std::list<Object*>::iterator it = itemList.begin(); it != itemList.end(); it++)
	{
		(*it)->draw(player, &allObjects, dt);
	}
	return;
}

void SolarSystem::addItem(Object * item)
{
	itemList.push_front(item);
	return;
}

void SolarSystem::addPlayerItem(Object * item)
{
	if (itemList.empty())
		itemList.push_front(item);
	else
		itemList.push_back(item);
	return;
}

void SolarSystem::removePlayerItem()
{
	itemList.pop_back();
	return;
}

void SolarSystem::deleteItems()
{
	itemList.clear();
}
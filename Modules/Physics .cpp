//SolarSystem.cpp

#include "stdafx.h"

#include "Physics.h"

using namespace cv;

void Physics::updatePositions(std::list<Object*>& starList, std::list<Object*>& itemList, float dt)
{
	for (std::list<Object*>::iterator it = starList.begin(); it != starList.end(); it++)
	{
		if (!(*it)->satelliteMap.empty())
		{
			Vec3f accMovement(0,0,0);
			(*it)->satMapUpdate(accMovement, dt);
		}		
	}

	Vec3f dl(0,0,0);
	for (std::list<Object*>::iterator it = itemList.begin(); it != itemList.end(); it++)
	{
		dl = (*it)->velocity*dt;
		(*it)->update(dl);
		//std::cout << "adkgha" << std::endl;
	}
	return;
}
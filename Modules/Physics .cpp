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
			(*it)->satMapUpdate(massPosList, accMovement, dt);
		}		
	}

	Vec3f dl(0,0,0);
	for (std::list<Object*>::iterator it = itemList.begin(); it != itemList.end(); it++)
	{
		(*it)->velocity += updateVelocities(itemList, (*it)->position, dt);
		dl = (*it)->velocity*dt;
		(*it)->update(dl);

	}
	massPosList.clear();
	return;
}

Vec3f Physics::updateVelocities(std::list<Object*>& itemList, cv::Vec3f position, float dt)
{
	Vec3f accDV(0,0,0);
	for (std::map<float, cv::Vec3f>::iterator it = massPosList.begin(); it != massPosList.end(); it++)
	{
		accDV += normalize(it->second - position) * gravity * it->first / norm(it->second - position)*dt;
	}
	return accDV;

}

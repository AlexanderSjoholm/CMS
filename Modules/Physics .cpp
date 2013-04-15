//SolarSystem.cpp

#include "stdafx.h"

#include "Physics.h"

using namespace cv;

void Physics::updatePositions(std::list<Object>& planetList, std::list<Object>& itemList, float dt)
{
	for (std::list<Object>::iterator it = planetList.begin(); it != planetList.end(); it++)
	{
		if (it->orbits)
		{
			float speed = norm(it->velocity);
			Vec3f relPos = normalize(it->position - it->orbits->position);
			relPos += relPos.cross(normalize(it->velocity))*dt*speed/it->distance;
			normalize(relPos);
			it->update(relPos * it->distance - it->position + it->orbits->position, Vec3f(0,0,0), Vec3f(0,0,0));
			std::cout << it->position << std::endl;
		}
		else
		{
			it->update(it->velocity*dt);
		}
	}
	return;
}
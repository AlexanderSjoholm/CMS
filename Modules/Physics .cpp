//SolarSystem.cpp

#include "stdafx.h"

#include "Physics.h"

using namespace cv;

void Physics::updatePositions(std::list<Object*>& starList, std::list<Object>& itemList, float dt)
{
	for (std::list<Object*>::iterator it = starList.begin(); it != starList.end(); it++)
	{
		if (!(*it)->satelliteMap.empty())
		{
			Vec3f accMovement(0,0,0);
			(*it)->satMapUpdate(accMovement, dt);
			
		}		
		/*
		if (it->orbits)
		{
			float speed = norm(it->velocity);
			Vec3f relPos = normalize(it->position - it->orbits->position);
			relPos += relPos.cross(normalize(it->velocity))*dt*speed/it->distance;
			normalize(relPos);
			Vec3f relMovement = (relPos * it->distance - it->position + it->orbits->position)
			it->update(relPos * it->distance - it->position + it->orbits->position);
			//it->setPosition(relPos * it->distance + it->orbits->position);
			if(!it->satelliteMap.empty())
			{
				for(std::map<float, Object>::iterator it = )
			}
		}
		else
		{
			it->update(it->velocity*dt);
		}*/
	}
	return;
}
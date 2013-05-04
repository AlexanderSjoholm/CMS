//SolarSystem.cpp

#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include "Physics.h"

using namespace cv;

void Physics::updatePositions(std::list<Object*>& starList, std::list<Object*>& itemList, float dt)
{
	for (std::list<Object*>::iterator it = starList.begin(); it != starList.end(); it++)
	{
		if (!(*it)->satelliteMap.empty())
		{
			Vec3f accMovement(0,0,0);
			(*it)->satMapUpdate(allPlanets, accMovement, dt);
		}		
	}

	Vec3f dl(0,0,0);
	for (std::list<Object*>::iterator it = itemList.begin(); it != itemList.end(); it++)
	{
		(*it)->velocity += updateVelocities(itemList, (*it)->position, dt);
		detectCollisions(itemList, *it);
		dl = (*it)->velocity*dt;
		(*it)->update(dl);
	}
	allPlanets.clear();
	return;
}

Vec3f Physics::updateVelocities(std::list<Object*>& itemList, cv::Vec3f position, float dt)
{
	Vec3f accDV(0,0,0);
	for (std::list<Object*>::iterator it = allPlanets.begin(); it != allPlanets.end(); it++)
	{
		accDV += normalize((*it)->position - position) * gravity * (*it)->mass / pow(norm((*it)->position - position), 2)*dt;
	}
	return accDV;

}

void Physics::detectCollisions(std::list<Object*>& itemList, Object* object)
{
	float radius = object->scale(1);
	for (std::list<Object*>::iterator it = allPlanets.begin(); it != allPlanets.end(); it++)
	{
		Vec3f distVec = (*it)->position - object->position;
		
		if (norm(distVec) < radius + (*it)->scale(1) && object->velocity.dot(distVec) > 0)
		{
			object->velocity += object->velocity.dot(normalize(distVec)) * normalize(distVec) * -2;
		}
	}
	for (std::list<Object*>::iterator it = itemList.begin(); it != itemList.end(); it++)
	{
		Vec3f distVec = (*it)->position - object->position;
		if (norm(distVec) < radius + (*it)->scale(1) && object->velocity.dot((*it)->velocity) < 0)
		{
			cv::Vec3f tempV = object->velocity;
			object->velocity += object->velocity.dot(normalize((*it)->velocity)) * normalize((*it)->velocity) * -2;
			(*it)->velocity += (*it)->velocity.dot(normalize(tempV)) * normalize(tempV) * -2;
		}
	}

	return;
}


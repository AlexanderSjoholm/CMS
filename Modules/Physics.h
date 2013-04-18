//SolarSystem.cpp

#include "Object.h"
#include "Player.h"

#ifndef PHYSICS_H
#define PHYSICS_H

class Physics
{
public:
	Physics() {};
	void updatePositions(std::list<Object*>& starList, std::list<Object>& itemList, float dt);

private:

};

#endif
//SolarSystem.cpp

//#include <opencv\cxcore.h>
#include "Object.h"
#include "Player.h"

#ifndef PHYSICS_H
#define PHYSICS_H


class Physics
{
public:
	Physics() {};
	void updatePositions(std::list<Object*>& starList, std::list<Object*>& itemList, float dt);

private:
	cv::Vec3f updateVelocities(std::list<Object*>& itemList,  cv::Vec3f position, float dt);
	
	std::map<float, cv::Vec3f> massPosList;

	static const int gravity = 10;
};

#endif
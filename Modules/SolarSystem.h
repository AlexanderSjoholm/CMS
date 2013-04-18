//SolarSystem.h

#include "Object.h"
#include "Player.h"
#include "Physics.h"

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H


struct Light
{
	cv::Vec3f * position;
	cv::Vec3b color;
	bool isDirectional;
};

class SolarSystem
{
public:
	SolarSystem() {};
	void addStar(Object * planet, bool isLightSource = false, cv::Vec3b color = 0);
	void addPlanet(Object * planet, bool isLightSource = false, cv::Vec3b color = 0);
	void update(Physics & PhysicsEngine, float dt);
	void draw(Player & player);
	//void addStaticLight(cv::Vec3b color);
	void addItem(Object * item);

private:
	std::list<Object*> starList, planetList; 
	std::list<Object> itemList;
	std::list<Light> lightList;
};

#endif
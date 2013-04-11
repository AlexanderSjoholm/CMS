//SolarSystem.h

#include "Object.h"
#include "Player.h"

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H


struct Light
{
	cv::Mat position;
	cv::Vec3b color;
	bool isDirectional;
};

class SolarSystem
{
public:
	SolarSystem() {};
	void addPlanet(Object * planet, bool isLightSource = true, cv::Vec3b color = 0);
	void update();
	void draw(Player & player);
	//void addStaticLight(cv::Vec3b color);
	void addItem(Object * item);

private:
	std::list<Object> planetList, itemList;
	std::list<Light*> lightList;
};

#endif
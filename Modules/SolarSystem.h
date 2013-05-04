//SolarSystem.h

#include "Object.h"
#include "Player.h"
#include "Physics.h"

#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H


struct Light
{
	cv::Vec3f * position;
	cv::Vec3f color;
	bool isDirectional;
};

class SolarSystem
{
public:
	SolarSystem() {};
	void addStar(Object * star, bool isLightSource = false, cv::Vec3f color = 0);
	void update(Physics & PhysicsEngine, float dt);
	void draw(Player * player);
	//void addStaticLight(cv::Vec3b color);
	void addItem(Object * item);
	void addPlayerItem(Object * item);
	void removePlayerItem();
	void deleteItems();
	void getObjects(std::list<Object*>* allObjects);
	
	std::list<Object*> starList; 

private:

	std::list<Object*> itemList;
	std::list<Light> lightList;
};

#endif
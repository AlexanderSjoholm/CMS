// Object.h, An instanciation of a model

#ifndef OBJECT_H
#define OBJECT_H
#include "opencv\cxcore.hpp"
#include "Model.h"
#include "Player.h"

class Object 
{
public:
	
	Object();
	void init(	Model* model, GLuint _program, GLchar* _vertexAttributeName, GLchar* _normalAttributName);
	void draw(Player* player);
	void update(cv::Vec3f position,		
				cv::Vec3f scale,	
				cv::Vec3f rotAngles);	
	void update(cv::Vec3f dl);
	void satMapUpdate(cv::Vec3f accMovement, float dt);

	void set(cv::Vec3f position, 
				cv::Vec3f scale,		
				cv::Vec3f rotAngles,	
				cv::Vec3f velocity);
	void setPosition(cv::Vec3f _position);

	void addSatellite(Object * object, float distance);

	void setOrbit(Object* orbits, float distance);
	
	cv::Vec3f position;
	cv::Vec3f scale;
	cv::Vec3f rotAngles;
	cv::Vec3f velocity;
	Object* orbits;
	Model* model;
	float distance;
	std::map<float, Object*> satelliteMap;
private:
	void updateMatrices();
	GLfloat transX, transY, transZ, 
			scaleX, scaleY, scaleZ,
			angX, angY, angZ,
			speedX, speedY, speedZ;

	//cv::Mat scale;
	
	cv::Mat scaleTrans, rotX, rotY, rotZ;
};

#endif
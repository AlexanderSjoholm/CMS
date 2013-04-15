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

	void set(cv::Vec3f position, 
				cv::Vec3f scale,		
				cv::Vec3f rotAngles,	
				cv::Vec3f velocity);
	void setPosition(cv::Vec3f _position);


	void setOrbit(Object* orbits, double distance);
	
	cv::Vec3f position;
	cv::Vec3f scale;
	cv::Vec3f rotAngles;
	cv::Vec3f velocity;
	Object* orbits;
	Model* model;
	double distance;
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
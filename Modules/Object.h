// Object.h, An instanciation of a model

#ifndef OBJECT_H
#define OBJECT_H

//#include "opencv\cxcore.hpp"
#include "Model.h"
#include "Player.h"

class Object 
{
public:
	
	Object();

	void init(	Model* model, GLuint _program, 
				cv::Vec4f shaderParameters = cv::Vec4f(0.3f, 0.4f, 0.3f, 10),
				GLuint _texture0 = 0, GLuint _texture1 = 0, GLuint _specularityMap = 0);

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

	GLuint program, texture0, texture1, bumpMap, specularityMap;
	GLchar *vertexAttributeName, *normalAttributeName, *texCoordAttributeName, *colourAttributeName;
	GLfloat ambientCoeff, diffuseCoeff, specularCoeff;
	GLuint  specularExponent;
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
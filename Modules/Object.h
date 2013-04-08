// Object.h, An instanciation of a model

#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"
#include "Player.h"

class Object 
{
public:
	
	Object();
	void init(	my::Model* model);
	void draw(Player* player);
	void update(GLfloat transX, GLfloat transY, GLfloat transZ, 
				GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ,
				GLfloat angX, GLfloat angY, GLfloat angZ);

	void set(	GLfloat transX, GLfloat transY, GLfloat transZ, 
				GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ,
				GLfloat angX, GLfloat angY, GLfloat angZ);
	
	my::Model* model;
private:
	void updateMatrices();
	GLfloat transX, transY, transZ, 
			scaleX, scaleY, scaleZ,
			angX, angY, angZ;

	cv::Mat scaleTrans, rotX, rotY, rotZ;
};

#endif
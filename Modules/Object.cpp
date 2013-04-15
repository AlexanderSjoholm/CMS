#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include "Object.h"
#include "myUtils.h"

Object::Object()
{

}

void Object::init(	Model* _model, GLuint _program, GLchar* _vertexAttributeName, GLchar* _normalAttributeName)
{
	model = _model;
	model->vertexAttributeName = _vertexAttributeName;
	model->normalAttributeName = _normalAttributeName;
	model->program = _program;
	
	position = cv::Vec3f(0, 0, 0);
	velocity = cv::Vec3f(0, 0, 0);
	scale = cv::Vec3f(0, 0, 0);
	rotAngles = cv::Vec3f(0, 0, 0);
	orbits = NULL;
	distance = 0;

	set(position, scale, rotAngles, velocity);
}

void Object::draw(Player* player)
{
	glUseProgram(model->program);
	
	glUniformMatrix4fv(glGetUniformLocation(model->program, "scaleTrans"), 1, GL_TRUE, scaleTrans.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "rotX"), 1, GL_TRUE, rotX.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "rotY"), 1, GL_TRUE, rotY.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "rotZ"), 1, GL_TRUE, rotZ.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	
	player->lookAtUpload(model->program);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(model->VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//std::cout << model->numberOfIndices << std::endl;
	glDrawElements(GL_TRIANGLES, model->numberOfIndices, GL_UNSIGNED_INT, 0);
}

void Object::update(cv::Vec3f _position,
					cv::Vec3f _scale,		
					cv::Vec3f _rotAngles)
{
	position += _position;
	scale += _scale;
	rotAngles += _rotAngles;

	updateMatrices();
}

void Object::update(cv::Vec3f _dl)
{
	position += _dl;

	updateMatrices();
}

void Object::setOrbit(Object* _orbits, double _distance)
{
	orbits = _orbits;
	distance = _distance;
}

void Object::setPosition(cv::Vec3f _position)
{
	position = _position;
	updateMatrices();
}

void Object::set(cv::Vec3f _position,	
				 cv::Vec3f _scale,	
				 cv::Vec3f _rotAngles,
				 cv::Vec3f _velocity)
{
	position = _position;
	scale = _scale;
	rotAngles = _rotAngles;
	velocity = _velocity;
	
	updateMatrices();
}

void Object::updateMatrices()
{
	GLfloat	scaleTransData[] = {	scale(0), 0.0f,	0.0f,	position(0),
									0.0f,	scale(1),	0.0f,	position(1),
									0.0f,	0.0f,	scale(2), position(2),
									0.0f,	0.0f,	0.0f,	1.0f };
	scaleTrans = cv::Mat(4, 4, CV_32FC1, scaleTransData).clone();
	
	GLfloat	rotXData[] = {	1.0f, 0.0f,			0.0f,		0.0f,
							0.0f, cos(rotAngles(0)),	-sin(rotAngles(0)),	0.0f,
							0.0f, sin(rotAngles(0)),	cos(rotAngles(0)),	0.0f,
							0.0f, 0.0f,			0.0f,		1.0f }; 
	rotX = cv::Mat(4, 4, CV_32FC1, rotXData).clone();

	GLfloat rotYData[] = {	cos(rotAngles(1)),	0.0f,	sin(rotAngles(1)),	0.0f,
							0.0f,		1.0f,	0.0f,		0.0f,
							-sin(rotAngles(1)),	0.0f,	cos(rotAngles(1)),	0.0f,
							0.0f,		0.0f,	0.0f,		1.0f };
	rotY = cv::Mat(4, 4, CV_32FC1, rotYData).clone();

	GLfloat rotZData[] = {	cos(rotAngles(2)),	-sin(rotAngles(2)),	0.0f, 0.0f,
							sin(rotAngles(2)),	cos(rotAngles(2)),	0.0f, 0.0f,
							0.0f,		0.0f,		1.0f, 0.0f,
							0.0f,		0.0f,		0.0f, 1.0f }; 
	rotZ = cv::Mat(4, 4, CV_32FC1, rotZData).clone();
}
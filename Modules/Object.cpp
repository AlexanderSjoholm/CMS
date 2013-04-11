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
	
	position = cv::Mat::zeros(3, 1, CV_32FC1);
	scale = cv::Mat::eye(3, 3, CV_32FC1);
	
	transX = 0.0f;
	transY = 0.0f;
	transZ = 0.0f;

	scaleX = 1.0f;
	scaleY = 1.0f;
	scaleZ = 1.0f;

	angX = 0;
	angY = 0;
	angZ = 0;

	set(	transX, transY, transZ, 
			scaleX, scaleY, scaleZ,
			angX, angY, angZ);
}

void Object::draw(Player* player)
{
	glUniformMatrix4fv(glGetUniformLocation(model->program, "scaleTrans"), 1, GL_TRUE, scaleTrans.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "rotX"), 1, GL_TRUE, rotX.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "rotY"), 1, GL_TRUE, rotY.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "rotZ"), 1, GL_TRUE, rotZ.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(model->program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	
	player->lookAtUpload(model->program);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(model->VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//std::cout << model->numberOfIndices << std::endl;
	glDrawElements(GL_TRIANGLES, model->numberOfIndices, GL_UNSIGNED_INT, 0);
}

void Object::update(GLfloat _transX, GLfloat _transY, GLfloat _transZ, 
					GLfloat _scaleX, GLfloat _scaleY, GLfloat _scaleZ,
					GLfloat _angX, GLfloat _angY, GLfloat _angZ)
{
	position.at<float>(0) += _transX;
	position.at<float>(1) += _transY;
	position.at<float>(2) += _transZ;
	
	transX += _transX;
	transY += _transY;
	transZ += _transZ;

	scaleX += _scaleX;
	scaleY += _scaleY;
	scaleZ += _scaleZ;

	angX += _angX;
	angY += _angY;
	angZ += _angZ;

	updateMatrices();
}

void Object::set(	GLfloat _transX, GLfloat _transY, GLfloat _transZ, 
					GLfloat _scaleX, GLfloat _scaleY, GLfloat _scaleZ,
					GLfloat _angX, GLfloat _angY, GLfloat _angZ)
{
	position.at<float>(0) += _transX;
	position.at<float>(1) += _transY;
	position.at<float>(2) += _transZ;
	
	transX = _transX;
	transY = _transY;
	transZ = _transZ;

	scaleX = _scaleX;
	scaleY = _scaleY;
	scaleZ = _scaleZ;

	angX = _angX;
	angY = _angY;
	angZ = _angZ;

	updateMatrices();
}

void Object::updateMatrices()
{
	GLfloat	scaleTransData[] = {	scaleX, 0.0f,	0.0f,	transX,
									0.0f,	scaleY,	0.0f,	transY,
									0.0f,	0.0f,	scaleZ, transZ,
									0.0f,	0.0f,	0.0f,	1.0f };
	scaleTrans = cv::Mat(4, 4, CV_32FC1, scaleTransData).clone();
	
	GLfloat	rotXData[] = {	1.0f, 0.0f,			0.0f,		0.0f,
							0.0f, cos(angX),	-sin(angX),	0.0f,
							0.0f, sin(angX),	cos(angX),	0.0f,
							0.0f, 0.0f,			0.0f,		1.0f }; 
	rotX = cv::Mat(4, 4, CV_32FC1, rotXData).clone();

	GLfloat rotYData[] = {	cos(angY),	0.0f,	sin(angY),	0.0f,
							0.0f,		1.0f,	0.0f,		0.0f,
							-sin(angY),	0.0f,	cos(angY),	0.0f,
							0.0f,		0.0f,	0.0f,		1.0f };
	rotY = cv::Mat(4, 4, CV_32FC1, rotYData).clone();

	GLfloat rotZData[] = {	cos(angZ),	-sin(angZ),	0.0f, 0.0f,
							sin(angZ),	cos(angZ),	0.0f, 0.0f,
							0.0f,		0.0f,		1.0f, 0.0f,
							0.0f,		0.0f,		0.0f, 1.0f }; 
	rotZ = cv::Mat(4, 4, CV_32FC1, rotZData).clone();
}
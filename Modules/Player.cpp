#include "stdafx.h" // Gör att Kompilatorn hittar filen (derp)
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include "Player.h"

Player::Player()
{
	position		= cv::Vec3f(5, 0, 5);
	lookAtVector	= cv::Vec3f(0, 0, 0);
	upVector		= cv::Vec3f(0, 1, 0);

	sensitivity = 0.025f;
	movementSpeed = 10.0f;
}

void Player::init()
{


}

void Player::lookAtUpdate(float dt)
{
	cv::Vec3f n = cv::normalize(position - lookAtVector);

	cv::Vec3f u = cv::normalize(upVector.cross(n));

	cv::Vec3f v = n.cross(u);

	GLfloat r[16] = {   u[0], u[1], u[2], 0.0,
                        v[0], v[1], v[2], 0.0,
                        n[0], n[1], n[2], 0.0,
                        0.0, 0.0, 0.0, 1.0};
	cv::Mat rot(4, 4, CV_32FC1, r);

	GLfloat t[16] = {   1.0, 0.0, 0.0, -position[0],
                        0.0, 1.0, 0.0, -position[1],
                        0.0, 0.0, 1.0, -position[2],
                        0.0, 0.0, 0.0, 1.0};
	cv::Mat trans(4, 4, CV_32FC1, t);

	lookAtMatrix = rot*trans;
}

void Player::lookAtUpload(GLuint program)
{
	glUniformMatrix4fv(glGetUniformLocation(program, "lookAtMatrix"), 1, GL_TRUE, lookAtMatrix.ptr<GLfloat>());
}

void Player::moveUp(float dt)
{
	std::cout << "moveUp" << std::endl;
	cv::Vec3f prevPos = position;
	position += cv::normalize(upVector)*movementSpeed*dt;
	lookAtVector += position - prevPos;
	std::cout << position - prevPos << std::endl;
}

void Player::moveDown(float dt)
{
	std::cout << "moveDown" << std::endl;
	cv::Vec3f prevPos = position;
	position -= cv::normalize(upVector)*movementSpeed*dt;
	lookAtVector += position - prevPos;
	std::cout << position - prevPos << std::endl;
}

void Player::moveForward(float dt)
{
	std::cout << "W" << std::endl;
	position += cv::normalize(lookAtVector - position)*movementSpeed*dt;
	std::cout << position << std::endl;
}
void Player::moveBackward(float dt)
{
	std::cout << "S" << std::endl;
	position -= cv::normalize(lookAtVector - position)*movementSpeed*dt;
	std::cout << position << std::endl;
}
void Player::moveLeft(float dt)
{
	std::cout << "A" << std::endl;
	position -= cv::normalize( (lookAtVector - position).cross(upVector) )*movementSpeed*dt;
	std::cout << position << std::endl;
}
void Player::moveRight(float dt)
{
	std::cout << "D" << std::endl;
	position += cv::normalize( (lookAtVector - position).cross(upVector) )*movementSpeed*dt;
	std::cout << position << std::endl;
}


void Player::xLook(float dt, int dx)
{
	std::cout << "xlook " << dx << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = cv::normalize(lookAtDirection.cross(upVector));

	lookAtVector = position + cv::normalize(lookAtDirection + sensitivity*helpVector*dx)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}

void Player::yLook(float dt, int dy)
{
	std::cout << "ylook " << dy << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = lookAtDirection.cross(upVector);

	lookAtVector = position + cv::normalize(lookAtDirection + sensitivity*upVector*dy)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}

// Old functions no longer needed (don't actually depend on dt)
void Player::lookUp(float dt)
{
	std::cout << "Up" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = lookAtDirection.cross(upVector);

	lookAtVector = position + cv::normalize(lookAtDirection + sensitivity*upVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
void Player::lookDown(float dt)
{
	std::cout << "Down" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = lookAtDirection.cross(upVector);

	lookAtVector = position + cv::normalize(lookAtDirection - sensitivity*upVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
void Player::lookLeft(float dt)
{
	std::cout << "Left" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = cv::normalize(lookAtDirection.cross(upVector));

	lookAtVector = position + cv::normalize(lookAtDirection - sensitivity*helpVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
void Player::lookRight(float dt)
{
	std::cout << "Right" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = cv::normalize(lookAtDirection.cross(upVector));

	lookAtVector = position + cv::normalize(lookAtDirection + sensitivity*helpVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
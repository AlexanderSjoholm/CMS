#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	position		= cv::Vec3f(5, 0, 5);
	lookAtVector	= cv::Vec3f(0, 0, 0);
	upVector		= cv::Vec3f(0, 1, 0);

	sensitivity = 0.2f;
	movementSpeed = 0.4f;
}

void Player::init()
{

}

void Player::lookAtUpdate()
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

void Player::moveForward()
{
	std::cout << "W" << std::endl;
	position += cv::normalize(lookAtVector - position)*movementSpeed;
	std::cout << position << std::endl;
}
void Player::moveBackward()
{
	std::cout << "S" << std::endl;
	position -= cv::normalize(lookAtVector - position)*movementSpeed;
	std::cout << position << std::endl;
}
void Player::moveLeft()
{
	std::cout << "A" << std::endl;
	position -= cv::normalize( (lookAtVector - position).cross(upVector) )*movementSpeed;
	std::cout << position << std::endl;
}
void Player::moveRight()
{
	std::cout << "D" << std::endl;
	position += cv::normalize( (lookAtVector - position).cross(upVector) )*movementSpeed;
	std::cout << position << std::endl;
}
void Player::lookUp()
{
	std::cout << "Up" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = lookAtDirection.cross(upVector);

	lookAtVector = position + cv::normalize(lookAtDirection + sensitivity*upVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
void Player::lookDown()
{
	std::cout << "Down" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = lookAtDirection.cross(upVector);

	lookAtVector = position + cv::normalize(lookAtDirection - sensitivity*upVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
void Player::lookLeft()
{
	std::cout << "Left" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = cv::normalize(lookAtDirection.cross(upVector));

	lookAtVector = position + cv::normalize(lookAtDirection - sensitivity*helpVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
void Player::lookRight()
{
	std::cout << "Right" << std::endl;
	cv::Vec3f lookAtDirection = lookAtVector - position;
	GLfloat length = (GLfloat)cv::norm(lookAtDirection);
	cv::Vec3f helpVector = cv::normalize(lookAtDirection.cross(upVector));

	lookAtVector = position + cv::normalize(lookAtDirection + sensitivity*helpVector)*length;
	upVector = cv::normalize(helpVector.cross(lookAtVector - position));
}
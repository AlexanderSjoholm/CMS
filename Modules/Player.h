// Player.h, Container for player related actions and information

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	
	Player();
	void init();
	void lookAtUpdate();
	void lookAtUpload(GLuint program);

	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void lookUp();
	void lookDown();
	void lookLeft();
	void lookRight();

	cv::Vec3f position, lookAtVector, upVector;
	cv::Mat lookAtMatrix;
	GLfloat sensitivity, movementSpeed;
private:
	

};

#endif
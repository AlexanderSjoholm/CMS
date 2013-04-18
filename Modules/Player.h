// Player.h, Container for player related actions and information

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
public:
	
	Player();
	void init();
	void lookAtUpdate(float dt);
	void lookAtUpload(GLuint program);

	void moveForward(float dt);
	void moveBackward(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);
	void moveUp(float dt);
	void moveDown(float dt);
	void lookUp(float dt);
	void lookDown(float dt);
	void lookLeft(float dt);
	void lookRight(float dt);


	void xLook(float dt, int dx);
	void yLook(float dt, int dy);


	cv::Vec3f position, lookAtVector, upVector;
	cv::Mat lookAtMatrix;
	GLfloat sensitivity, arrowSensitivity, movementSpeed;
private:
	

};

#endif
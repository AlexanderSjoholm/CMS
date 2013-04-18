#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include "Object.h"
#include "myUtils.h"

using namespace cv;

Object::Object()
{

}

void Object::init(	Model* _model, GLuint _program, GLchar* _vertexAttributeName, GLchar* _normalAttributeName, GLchar* _texCoordAttributeName, GLuint _texture0, GLuint _texture1)
{
	model = _model;

	program = _program;
	vertexAttributeName = _vertexAttributeName;
	normalAttributeName = _normalAttributeName;
	texCoordAttributeName = _texCoordAttributeName;

	texture0 = _texture0;
	texture1 = _texture1;

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
	for(std::map<float, Object*>::iterator it = satelliteMap.begin(); it != satelliteMap.end(); ++it)
	{
		it->second->draw(player);
	}
	
	glUseProgram(program);
	
	// Upload Transformations
	glUniformMatrix4fv(glGetUniformLocation(program, "scaleTrans"), 1, GL_TRUE, scaleTrans.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(program, "rotX"), 1, GL_TRUE, rotX.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(program, "rotY"), 1, GL_TRUE, rotY.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(program, "rotZ"), 1, GL_TRUE, rotZ.ptr<GLfloat>());
	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	
	player->lookAtUpload(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "rotZ"), 1, GL_TRUE, rotZ.ptr<GLfloat>());

	// Bind the right textures
	if (texture0 != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(program, "Tex0"), 0);

		if (texture1 != 0)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture1);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glUniform1i(glGetUniformLocation(program, "Tex1"), 1);
		}
	}

	// Bind the model's VAO and buffers to the program specified by the object
	glBindVertexArray(model->VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glVertexAttribPointer(glGetAttribLocation(program, vertexAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, vertexAttributeName));

	glBindBuffer(GL_ARRAY_BUFFER, model->NBO);
	glVertexAttribPointer(glGetAttribLocation(program, normalAttributeName), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, normalAttributeName));

	glBindBuffer(GL_ARRAY_BUFFER, model->TBO);
	glVertexAttribPointer(glGetAttribLocation(program, texCoordAttributeName), 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, texCoordAttributeName));
	
	glDrawElements(GL_TRIANGLES, model->numberOfIndices, GL_UNSIGNED_INT, 0);
}

void Object::addSatellite(Object * object, float _distance)
{
	object->orbits = this;
	object->distance = _distance;
	satelliteMap.insert(std::pair<float, Object*>(object->distance, object));
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

void Object::satMapUpdate(Vec3f _accMovement, float dt)
{
	if (orbits)
	{
		float speed = norm(velocity);
		//std::cout << "whyyy " << position - orbits->position << std::endl;
		Vec3f relPos = normalize(position - orbits->position);
		//std::cout << "distance " << distance << std::endl;
		relPos += relPos.cross(normalize(velocity))*dt*speed/distance;
		normalize(relPos);
		Vec3f accMovement = (relPos * distance - position + orbits->position) + _accMovement;
		 
		update(accMovement);
			
		for(std::map<float, Object*>::iterator it = satelliteMap.begin(); it != satelliteMap.end(); ++it)
		{
			it->second->satMapUpdate(accMovement, dt);
		}
	}
	else 
	{
		for(std::map<float, Object*>::iterator it = satelliteMap.begin(); it != satelliteMap.end(); ++it)
		{
			it->second->satMapUpdate(_accMovement, dt);
		}
	}

}

void Object::update(cv::Vec3f _dl)
{
	position += _dl;

	updateMatrices();
}

void Object::setOrbit(Object* _orbits, float _distance)
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
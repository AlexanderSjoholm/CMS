#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include "Object.h"
#include "myUtils.h"

using namespace cv;

Object::Object()
{

}

void Object::init(	Model* _model, GLuint _program, cv::Vec4f shaderParametes, GLuint _texture0, GLuint _texture1, GLuint _specularityMap, GLuint _normalMap)
{
	model = _model;

	program = _program;
	vertexAttributeName = "inPosition";
	normalAttributeName = "inNormal";
	texCoordAttributeName = "inTexCoord";

	ambientCoeff = shaderParametes[0];
	diffuseCoeff = shaderParametes[1];
	specularCoeff = shaderParametes[2];
	specularExponent = (GLuint)shaderParametes[3];

	texture0 = _texture0;
	texture1 = _texture1;
	specularityMap = _specularityMap;
	normalMap = _normalMap;

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

	// Upload shading parameters
	glUniform1fv(glGetUniformLocation(program, "ambientCoeff"), 1, &ambientCoeff);
    glUniform1fv(glGetUniformLocation(program, "diffuseCoeff"), 1, &diffuseCoeff);
    glUniform1fv(glGetUniformLocation(program, "specularCoeff"), 1, &specularCoeff);
    glUniform1uiv(glGetUniformLocation(program, "specularExponent"), 1, &specularExponent);
	
	glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, cv::Mat(player->position).ptr<GLfloat>());
	
	//glUniformMatrix4fv(glGetUniformLocation(program, "rotZ"), 1, GL_TRUE, rotZ.ptr<GLfloat>());

	

	// Bind the right textures
	if (texture0 != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(program, "Tex0"), 0);
	}
	
	if (texture1 != 0)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(program, "Tex1"), 1);
	}
	
	if (specularityMap != 0)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, specularityMap);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(program, "Tex2"), 2);
	}

	if (normalMap != 0)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(program, "Tex3"), 3);
	}

	if (noiseTexture != 0)
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glUniform1i(glGetUniformLocation(program, "Tex4"), 4);
	}

	float randomVector[] = {normalDistribution(generator), normalDistribution(generator), normalDistribution(generator)};
	glUniform3fv(glGetUniformLocation(program, "noise"), 1, randomVector);

	// Upload lightsource


	// Upload PlanetPositions and radius
	float planetPosition[] = {0, 0, -10};
	glUniform3fv(glGetUniformLocation(program, "planetPosition"), 1, planetPosition);

	float planetRadius = 2;
	glUniform1fv(glGetUniformLocation(program, "planetRadius"), 1, &planetRadius);

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

void Object::satMapUpdate(std::map<float, cv::Vec3f>& massPosList, Vec3f _accMovement, float dt)
{
	if (orbits)
	{
		float speed = norm	(velocity);
		Vec3f relPos = normalize(position - orbits->position);
		relPos += relPos.cross(normalize(velocity))*dt*speed/distance;
		normalize(relPos);
		Vec3f accMovement = (relPos * distance - position + orbits->position) + _accMovement;
		update(accMovement);
		
		for(std::map<float, Object*>::iterator it = satelliteMap.begin(); it != satelliteMap.end(); ++it)
		{
			it->second->satMapUpdate(massPosList, accMovement, dt);
		}
	}
	else 
	{
		for(std::map<float, Object*>::iterator it = satelliteMap.begin(); it != satelliteMap.end(); ++it)
		{
			it->second->satMapUpdate(massPosList, _accMovement, dt);
		}
	}
	float mass = scale(0) * scale(1) * scale(2);
	massPosList.emplace(std::pair<float, cv::Vec3f>(mass, position));

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
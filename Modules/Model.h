// model.h, Definition of the model class

#ifndef MODEL_H
#define MODEL_H

namespace my
{

class Model 
{
public:
	
	Model();
	void init(	
				GLuint _program, 
				GLchar* _vertexAttributeName, 
				GLchar* _colourAttributeName);

	GLuint program, VAO, VBO, CBO;
	GLchar *vertexAttributeName, *colourAttributeName;
private:
	

};

}

#endif
// model.h, Definition of the model class

#ifndef MODEL_H
#define MODEL_H

class Model 
{
public:
	
	Model();
	void init(	
				GLuint _program, 
				GLchar* _vertexAttributeName, 
				GLchar* _colourAttributeName);
	void upload();

	GLuint program, VAO, VBO, NBO, IBO, CBO, numberOfVertices, numberOfIndices;
	GLchar *vertexAttributeName, *normalAttributeName, *colourAttributeName;
	std::vector<GLfloat> vertexArray, normalArray, texCoordArray;
	std::vector<unsigned int> indexArray;
private:
	

};
#endif
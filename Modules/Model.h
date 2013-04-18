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

	GLuint program, VAO, VBO, NBO, TBO, IBO, CBO, numberOfVertices, numberOfIndices;
	GLchar *vertexAttributeName, *normalAttributeName, *texCoordAttributeName, *colourAttributeName;
	
	std::vector<cv::Vec3f> vertexArray, normalArray;
	std::vector<cv::Vec2f> texCoordArray;
	std::vector<cv::Vec3i> indexArray;
private:
	

};
#endif
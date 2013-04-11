#include "stdafx.h"
#include "myLoadObj.h"

using namespace std;

void myLoadObj(char* filename, Model* model)
{
	ifstream in(filename); // , ios::in);
	if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }

	//vector<GLfloat>	vertices;
	//vector<GLfloat>	normals;
	//vector<int>		indices;

	string line;
	while (getline(in, line))
	{
		// Vertex
		if (line.substr(0, 2) == "v ")
		{
			istringstream ss(line.substr(2));
			GLfloat x, y, z;
			// x
			ss >> x;
			// y
			ss >> y;
			// z
			ss >> z;
			model->vertexArray.push_back(x);
			model->vertexArray.push_back(y);
			model->vertexArray.push_back(z);
			model->numberOfVertices++;
		}
		// Face
		else if (line.substr(0, 2) == "f ")
		{
			istringstream ss(line.substr(2));
			unsigned int a,b,c;

			// First Vertex
			ss >> a;

			// Ta bort allt tom nästa blankslag
			if (ss.peek() == '/')
			{
				string crap;
				ss >> crap; 
				//std::cout << crap << std::endl;
			}

			// Second Vertex
			ss >> b;

			// Ta bort allt tom nästa blankslag
			if (ss.peek() == '/')
			{
				string crap;
				ss >> crap; 
				//std::cout << crap << std::endl;
			}

			// Third Vertex
			ss >> c;

			// Ta bort allt tom nästa blankslag
			if (ss.peek() == '/')
			{
				string crap;
				ss >> crap; 
				//std::cout << crap << std::endl;
			}

			// .obj indeces start at 1. Vector start at 0
			a--;
			b--;
			c--;

			//std::cout << a << std::endl;
			//std::cout << b << std::endl;
			//std::cout << c << std::endl;

			model->indexArray.push_back(a);
			model->indexArray.push_back(b);
			model->indexArray.push_back(c);

			model->numberOfIndices += 3;
		}
		else if (line.substr(0, 2) == "vn")
		{
			istringstream ss(line.substr(2));
			GLfloat x, y, z;
			// x
			ss >> x;
			// y
			ss >> y;
			// z
			ss >> z;
			model->normalArray.push_back(x);
			model->normalArray.push_back(y);
			model->normalArray.push_back(z);
		}
		else if (line[0])
		{
			// This line is a comment
		}
		else
		{
			// Some case not suported at the moment
		}
	}

	// Put data into a Model
	//model->init();

	cout << filename << " is succsessfully loaded" << endl;
}
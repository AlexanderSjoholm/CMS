// myInit.cpp

#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)
#include <fstream>
#include "myInit.h"


using namespace std;

char* readFile2Char(char* file)
{
	ifstream s;
	int length;
	// Open stream to/from file
	s.open(file, ios::in);
	// Find length of file
	s.seekg(0, ios::end);
	length = (int)s.tellg();
	// Set pointer to begining of file
	s.seekg(0, ios::beg);

	char* buffer = new char[length];
	s.read(buffer, length);
	s.close();
	// Add zero termination
	buffer[length] = 0;
	
	return buffer;
}


void programInit(std::map<std::string, GLuint>& shaderMap,
				 std::map<std::string, Model*>& modelMap,
				 std::map<std::string, GLuint>& textureMap,
				 std::map<std::string, GLuint>& normalMap)
{
	uploadShaders(shaderMap);
	uploadModels(modelMap);
	uploadTextures(textureMap);
	uploadNormals(normalMap);
}

void uploadShaders(std::map<std::string, GLuint>& shaderMap)
{
	GLuint program, program2, program3, sunShader, earthShader, phongNoTex, phongTex, planetShader, planetDeluxeShader;
	
	shaderInit(&planetShader, "Shaders/planet.vert", "Shaders/planet.frag");
	shaderInit(&planetDeluxeShader, "Shaders/planetDeluxe.vert", "Shaders/planetDeluxe.frag");
	shaderInit(&program, "Shaders/test.vert", "Shaders/test.frag");
	shaderInit(&sunShader, "Shaders/sun.vert", "Shaders/sun.frag");
	shaderInit(&earthShader, "Shaders/earth.vert", "Shaders/earth.frag");
	shaderInit(&phongNoTex, "Shaders/phongNoTex.vert", "Shaders/phongNoTex.frag");
	shaderInit(&phongTex, "Shaders/phongTex.vert", "Shaders/phongTex.frag");

	shaderMap.emplace(std::pair<std::string, GLuint>("Planet", planetShader));
	shaderMap.emplace(std::pair<std::string, GLuint>("Planet deluxe", planetDeluxeShader));
	shaderMap.emplace(std::pair<std::string, GLuint>("Test", program));
	shaderMap.emplace(std::pair<std::string, GLuint>("Sun", sunShader));
	shaderMap.emplace(std::pair<std::string, GLuint>("Earth", earthShader));
	shaderMap.emplace(std::pair<std::string, GLuint>("Phong shading without texture", phongNoTex));
	shaderMap.emplace(std::pair<std::string, GLuint>("Phong shading with texture", phongTex));
}

void uploadModels(std::map<std::string, Model*>& modelMap)
{
	Model* sphereModel50 = new Model;
	generateSphere(sphereModel50, 50);						
	sphereModel50->upload();
	modelMap.emplace(std::pair<std::string, Model*>("HR Sphere", sphereModel50));
	
}

void uploadTextures(std::map<std::string, GLuint>& textureMap)
{
	GLuint sunTexture, mercuryTexture, venusTexture, earthTextureDay, earthTextureNight, moonTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture, plutoTexture;
	
	LoadTGATextureSimple("Textures/sunTexture.tga", &sunTexture);
	LoadTGATextureSimple("Textures/mercuryTexture.tga", &mercuryTexture);
	LoadTGATextureSimple("Textures/venusTexture.tga", &venusTexture);
	LoadTGATextureSimple("Textures/earthTextureDay.tga", &earthTextureDay);
	LoadTGATextureSimple("Textures/earthTextureNight.tga", &earthTextureNight);
	LoadTGATextureSimple("Textures/moonTexture.tga", &moonTexture);
	LoadTGATextureSimple("Textures/marsTexture.tga", &marsTexture);
	LoadTGATextureSimple("Textures/jupiterTexture.tga", &jupiterTexture);
	LoadTGATextureSimple("Textures/saturnTexture.tga", &saturnTexture);
	LoadTGATextureSimple("Textures/uranusTexture.tga", &uranusTexture);
	LoadTGATextureSimple("Textures/neptuneTexture.tga", &neptuneTexture);
	LoadTGATextureSimple("Textures/plutoTexture.tga", &plutoTexture);

	textureMap.emplace(std::pair<std::string, GLuint>("Sun", sunTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Mercury", mercuryTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Venus", venusTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Earth Day", earthTextureDay));
	textureMap.emplace(std::pair<std::string, GLuint>("Earth Night", earthTextureNight));
	textureMap.emplace(std::pair<std::string, GLuint>("Moon", moonTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Mars", marsTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Jupiter", jupiterTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Saturn", saturnTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Uranus", uranusTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Neptune", neptuneTexture));
	textureMap.emplace(std::pair<std::string, GLuint>("Pluto", plutoTexture));
}

void uploadNormals(std::map<std::string, GLuint>& normalMap)
{
	GLuint earthNormalMap, moonNormalMap, marsNormalMap, earthSpecularityMap;
	
	LoadTGATextureSimple("Textures/earthNormalMap.tga", &earthNormalMap);
	LoadTGATextureSimple("Textures/moonNormalMap.tga", &moonNormalMap);
	LoadTGATextureSimple("Textures/marsNormalMap.tga", &marsNormalMap);
	LoadTGATextureSimple("Textures/earthSpecularityMap.tga", &earthSpecularityMap);

	normalMap.emplace(std::pair<std::string, GLuint>("Earth", earthNormalMap));
	normalMap.emplace(std::pair<std::string, GLuint>("Moon", moonNormalMap));
	normalMap.emplace(std::pair<std::string, GLuint>("Mars", marsNormalMap));
	normalMap.emplace(std::pair<std::string, GLuint>("Earth Specularity", earthSpecularityMap));
}

void shaderInit(GLuint* program, std::string vertexPath, std::string fragmentPath)
{
	// Program and Shader Identifiers
	GLchar *vertexCode, *fragmentCode;
	GLuint vertexShader, fragmentShader;

	// Create shader objects
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Read shaders from external files
	//vertexCode = readFile2Char("Shaders/test.vert");
	//fragmentCode = readFile2Char("Shaders/test.frag");
	vertexCode = readFile2Char((char*)vertexPath.c_str());
	fragmentCode = readFile2Char((char*)fragmentPath.c_str());	
	// Print shaders just for fun
	//cout << vertexCode << endl << fragmentCode;
	glShaderSource(vertexShader, 1, (const GLchar **) &vertexCode, NULL);  
	glShaderSource(fragmentShader, 1, (const GLchar **) &fragmentCode, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	// Create an empty program to which shaders can be attached
	*program = glCreateProgram();
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);

	//glBindAttribLocation(*program, 0, "in_Position");
	//glBindAttribLocation(*program, 1, "in_Color");

	// Link program. Each shader attached will become executalbes to run shader processor.
	glLinkProgram(*program);
	glUseProgram(*program);
}

void GLInit()
{
	// GL inits
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void initPresetSystem(std::map<std::string, GLuint>& shaderMap,
		 			  std::map<std::string, Model*>& modelMap,
		 			  std::map<std::string, GLuint>& textureMap,
		 			  std::map<std::string, GLuint>& normalMap,
		 			  std::map<std::string, Object*>& presetMap)
{
	Object* sun = new Object(modelMap["HR Sphere"], shaderMap["Sun"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Sun"],0,0,0);
	presetMap.insert(std::pair<std::string, Object*>("Sun", sun->clone()));

	Object* earth = new Object(modelMap["HR Sphere"], shaderMap["Earth"], cv::Vec4f(0.2f, 0.5f, 0.5f, 3), textureMap["Earth Day"],textureMap["Earth Night"],normalMap["Earth specularity"], normalMap["Earth"]);
	presetMap.insert(std::pair<std::string, Object*>("Earth", earth->clone()));

	Object* moon = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Moon"], 0, 0, normalMap["Moon"]);
	presetMap.insert(std::pair<std::string, Object*>("Moon", moon->clone()));

	Object* mercury = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Mercury"], 0, 0, 0);
	presetMap.insert(std::pair<std::string, Object*>("Mercury", mercury->clone()));

	Object* venus = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Venus"], 0, 0, 0);
	presetMap.insert(std::pair<std::string, Object*>("Venus", venus->clone()));

	Object* mars = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Mars"], 0, 0, normalMap["Mars"]);
	presetMap.insert(std::pair<std::string, Object*>("Mars", mars->clone()));

	Object* jupiter = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Jupiter"], 0, 0, 0);
	presetMap.insert(std::pair<std::string, Object*>("Jupiter", jupiter->clone()));

	Object* saturn = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Saturn"], 0, 0, 0);
	presetMap.insert(std::pair<std::string, Object*>("Saturn", saturn->clone()));

	Object* uranus = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Uranus"], 0, 0, 0);
	presetMap.insert(std::pair<std::string, Object*>("Uranus", uranus->clone()));

	Object* neptune = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Neptune"], 0, 0, 0);
	presetMap.insert(std::pair<std::string, Object*>("Neptune", neptune->clone()));

	Object* pluto = new Object(modelMap["HR Sphere"], shaderMap["Planet"], cv::Vec4f(0.2f, 0.5f, 0.8f, 10), textureMap["Pluto"], 0, 0, 0);
	presetMap.insert(std::pair<std::string, Object*>("Pluto", pluto->clone()));

}



/*
	// Sun
	Object sun;
	GLuint sunTexture;
	LoadTGATextureSimple("Textures/sunTexture.tga", &sunTexture);
	sun.init(&sphereModel, sunShader, standardShaderParameters, sunTexture);
	sun.set(cv::Vec3f(0,0,0),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
	
	// Mercury
	Object mercury;
	GLuint mercuryTexture;
	LoadTGATextureSimple("Textures/mercuryTexture.tga", &mercuryTexture);
	mercury.init(&sphereModel, planetShader, standardShaderParameters, mercuryTexture);
	mercury.set(cv::Vec3f(-10,0,0),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	// Venus
	Object venus;
	GLuint venusTexture;
	LoadTGATextureSimple("Textures/venusTexture.tga", &venusTexture);
	venus.init(&sphereModel, planetShader, standardShaderParameters, venusTexture);
	venus.set(cv::Vec3f(-8,0,2),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
	
	// Earth
	Object earth;
	GLuint earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap;
	LoadTGATextureSimple("Textures/earthTextureDay.tga", &earthTextureDay);
	LoadTGATextureSimple("Textures/earthTextureNight.tga", &earthTextureNight);
	LoadTGATextureSimple("Textures/earthSpecularityMap.tga", &earthSpecularityMap);
	LoadTGATextureSimple("Textures/earthNormalMap.tga", &earthNormalMap);
	cv::Vec4f earthShaderParameters(0.2f, 0.5f, 0.5f, 3);
	earth.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	earth.set(cv::Vec3f(-10,0,-10),  cv::Vec3f(10,10,10), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
	
	// Moon
	Object moon;
	GLuint moonTexture, moonNormalMap;
	LoadTGATextureSimple("Textures/moonTexture.tga", &moonTexture);
	LoadTGATextureSimple("Textures/moonNormalMap.tga", &moonNormalMap);
	moon.init(&sphereModel, planetDeluxeShader, earthShaderParameters, moonTexture, 0, 0, moonNormalMap);
	moon.set(cv::Vec3f(-7,0,-7),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	// Mars
	Object mars;
	GLuint marsTexture, marsNormalMap;
	LoadTGATextureSimple("Textures/marsTexture.tga", &marsTexture);
	LoadTGATextureSimple("Textures/marsNormalMap.tga", &marsNormalMap);
	mars.init(&sphereModel, planetShader, standardShaderParameters, marsTexture, 0, 0, marsNormalMap);
	mars.set(cv::Vec3f(-6,0,4),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	// Jupiter
	Object jupiter;
	GLuint jupiterTexture;
	LoadTGATextureSimple("Textures/jupiterTexture.tga", &jupiterTexture);
	jupiter.init(&sphereModel, planetShader, standardShaderParameters, jupiterTexture);
	jupiter.set(cv::Vec3f(-4,0,6),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
	
	// Saturn
	Object saturn;
	GLuint saturnTexture;
	LoadTGATextureSimple("Textures/saturnTexture.tga", &saturnTexture);
	saturn.init(&sphereModel, planetShader, standardShaderParameters, saturnTexture);
	saturn.set(cv::Vec3f(-2,0,8),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	// Uranus
	Object uranus;
	GLuint uranusTexture;
	LoadTGATextureSimple("Textures/uranusTexture.tga", &uranusTexture);
	uranus.init(&sphereModel, planetShader, standardShaderParameters, uranusTexture);
	uranus.set(cv::Vec3f(0,0,10),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	// Neptune
	Object neptune;
	GLuint neptuneTexture;
	LoadTGATextureSimple("Textures/neptuneTexture.tga", &neptuneTexture);
	neptune.init(&sphereModel, planetShader, standardShaderParameters, neptuneTexture);
	neptune.set(cv::Vec3f(2,0,8),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	// Pluto
	Object pluto;
	GLuint plutoTexture;
	LoadTGATextureSimple("Textures/plutoTexture.tga", &plutoTexture);
	pluto.init(&sphereModel, planetShader, standardShaderParameters, plutoTexture);
	pluto.set(cv::Vec3f(4,0,6),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);


	
	ground.init(&groundModel, phongTex, standardShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, testTex);
	ground.set(cv::Vec3f(0,-1,0),  cv::Vec3f(10,10,10), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);

	sphere1.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	sphere1.set(cv::Vec3f(5,0,0),  cv::Vec3f(0.1,0.1,0.1), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
	
	sphere2.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	sphere2.set(cv::Vec3f(0,5,0),  cv::Vec3f(0.3,0.3,0.3), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
	
	sphere3.init(&sphereModel, earthShader, earthShaderParameters, earthTextureDay, earthTextureNight, earthSpecularityMap, earthNormalMap);
	sphere3.set(cv::Vec3f(0,0,5),  cv::Vec3f(0.9,0.9,0.9), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 1);
	
	
	square.set(cv::Vec3f(0,0,-40), cv::Vec3f(30,30,30), cv::Vec3f(1.5,0,0), cv::Vec3f(0,0,0), 1);
	square.update(cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), cv::Vec3f(-0.05f,0,0));
	earth.addSatellite(&moon, 3);
	sun.addSatellite(&earth, 10);
	solsystem.addStar(&sun, true, cv::Vec3f(1,1,1));
	solsystem.addStar(&earth, true, cv::Vec3f(1,1,1));
	*/
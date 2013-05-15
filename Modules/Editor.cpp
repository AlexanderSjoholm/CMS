#include "stdafx.h"
//#include "../../Repository/stdafx.h" // Gör att editorn hittar filen (suck)

#include "Editor.h"

#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

Editor::Editor(std::map<std::string, Model*> _modelMap,
			   std::map<std::string, GLuint> _shaderMap,
			   std::map<std::string, GLuint> _textureMap,
			   std::map<std::string, GLuint> _normalMap,
			   std::map<std::string, Object*> _presetMap)
{
	modelMap = _modelMap;
	shaderMap = _shaderMap;
	textureMap = _textureMap;
	normalMap = _normalMap;
	presetMap = _presetMap;
}


void Editor::edit(SolarSystem& solsystem, std::vector<bool>& states, Object* _currentObject)
{
	//initDefaultSystem(solsystem);
	Object* currentObject = _currentObject;
	//Object* currentObject = solsystem.starList.front();
	//currentObject = presetMap["Moon"];
	//bool inEditor = true;
	bool inEditor = false;
	if(states[STARTUP])
	{
		initDefaultSystem(solsystem);
		printWelcome();
		cout << "\n";
		printHelp();
	}
	do
	{
		cout << "  Entering editor, type HELP for information\n";
		std::string command;
		cin >> command;
		if(command == "HELP")
			printHelp();
		else if (command == "START")
			inEditor = false;
		else if (command == "DELETEALL")
			deleteSolarSystem(solsystem);
		else if (command == "DELETEITEMS")
			solsystem.deleteItems();
		else if (command == "DELETE")
			currentObject = deleteObject(solsystem, currentObject);
		else if (command == "RESET")
		{
			deleteSolarSystem(solsystem);
			initDefaultSystem(solsystem);
			cout << "Solarsystem successfully reset." << endl;
		}
		else if (command == "PLANETEDITOR")
			planetEditor(currentObject);
		else if (command == "PLANETCREATOR")
			planetCreator();
		else if (command == "TEXTURES")
			printTextures();
		else if (command == "SHADERS")
			printShaders();
		else if (command == "NORMALS")
			printNormals();
		else if (command == "PRESET")
			printPresets();
		else if (command == "EXIT")
			states[EDITOR] = states[RUNNING] = false;
		else
			cout << "Bad command, type \"help\" for avaliable commands" << endl;
	}
	while(inEditor);
}

void Editor::deleteSolarSystem(SolarSystem& solsystem)
{
	for(list<Object*>::iterator it = solsystem.starList.begin(); it != solsystem.starList.end(); ++it)
	{
		delete (*it);
	}
	solsystem.starList.clear();
}

Object* Editor::deleteObject(SolarSystem& solsystem, Object* _currentObject)
{
	Object* orbits = _currentObject->orbits;
	if(orbits)
	{
		float distance = _currentObject->distance;
		delete orbits->satelliteMap[distance];
		orbits->satelliteMap.erase(distance);
		return orbits;
	}
	else
	{
		for(list<Object*>::iterator it = solsystem.starList.begin(); it != solsystem.starList.end(); ++it)
		{
			if ((*it) == _currentObject)
			{
				delete *it;
			}
		}
		solsystem.starList.clear();
		return NULL;
	}
}

void Editor::planetEditor(Object* currentObject)
{
	if(!currentObject)
	{
		cout << "No object selected, please exit the editor and right-click a planet, star or moon to select it.\n";
		return;
	}

	bool inPlanetEditor = true;

	Object* newObject;
	do
	{
		cout << "  Welcome to the planet editor, Here you can add or remove the \n"
			 << "  satellites of your selected planet, type \"EXIT\" to exit.\n"
			 << "  To start, select a preset planet by writing \"ADDPRESET\" followed \n"
			 << "  by the preset name or create a new preset planet by typing \"NEW\".\n";
		std::string command;
		cin >> command;
		if(command == "HELP")
			printHelp();
		else if (command == "NEW")
		{
			planetCreator();
		}
		else if (command == "ADDPRESET")
		{
			printPresets();
			cin >> command;
			std::map<std::string, Object*>::iterator it = presetMap.find(command);
			if (it == presetMap.end()) 
				cout << "preset not found, try again by typing \"ADDPRESET\": \n";
			else
			{
				newObject = it->second->clone();
				setNewObject(newObject, currentObject);
				inPlanetEditor = false;
			}
				
		}
		else if (command == "DELETE")
		{
			cout << "Not supported yet...\n";
			if (currentObject->satelliteMap.size() == 0)
			{
				cout << "Object has no satellites\n";
			}
			else
			{
				cout << "Which satellite do you want removed?\n";
				int satellite;
				cin >> satellite;
				cout << "remove satellite # " << satellite << endl;
			}
		}
		else if (command == "EXIT")
			inPlanetEditor = false;
		else
			cout << "Bad command, try again\n" ;
		
		inPlanetEditor = false;
	}
	while (inPlanetEditor);
}

void Editor::planetCreator()
{
	bool inPlanetCreator = true;
	bool badInput = false;
	std::string input, temp; 
	GLuint texture1, texture2, shader, normals;
	texture1 = texture2 = shader = normals = 0;
	Model* model = modelMap["HR Sphere"];
	cv::Vec4f shaderParams, color(0,0,0,0);
	std::map<std::string, GLuint>::iterator it;

	do
	{
		if (badInput == true)
			cout << "  Bad input, please start over\n";
		
		badInput = false;
		cout << "  Welcome to the planet creator, Here you can create a custom planet.\n"
			 << "  First, choose a shader program:\n";
		printShaders();
		cin >> input;
		
		if (input == "EXIT")
			return;
		else
			it = shaderMap.find(input);
				
		if (it == shaderMap.end()) 
			badInput = true;

		if(!badInput)
		{
			shader = it->second;
			cout << "  Shader found, now choose two textures or type \"SKIP\"\n";
			printTextures();
			getline(cin, input);
			cin >> temp;
			it = textureMap.find(temp);
			if (it == textureMap.end()) 
				cout << "  No texture selected.\n";
			else
			{
				texture1 = it->second;
				cout << "  Texture 1 loaded.\n";
				if(cin >> temp)	
				{
					it = textureMap.find(temp);
					if (it != textureMap.end())
					{
						texture2 = it->second;
						cout << "  Texture 2 loaded.\n";
					}
				}
			}
		}
		if(!badInput)
		{
			cout << "  Textures selected, now choose normal map or type \"SKIP\"\n";
			cin >> input;
			it = normalMap.find(input);
			if(it != normalMap.end())
				normals = it->second;
		}
		
		if(!badInput)
		{
			cout << "  Normals selected, now choose color [0,1] as \"R G B A\" or type \"SKIP\"\n";
			getline(cin, input);
			if(!(cin >> color[0]))
				badInput = true;
			if(!(cin >> color[1]))
				badInput = true;
			if(!(cin >> color[2]))
				badInput = true;
			if(!(cin >> color[3]))
				badInput = true;
		}
		
		if(!badInput)
		{
			cout << "  Normals selected, now choose shader params as \n"
				 << "  \"ac dc sc se\"\n";
			getline(cin, input);
			if(!(cin >> shaderParams[0]))
				badInput = true;
			if(!(cin >> shaderParams[1]))
				badInput = true;
			if(!(cin >> shaderParams[2]))
				badInput = true;
			if(!(cin >> shaderParams[3]))
				badInput = true;
		}

		if(!badInput)
		{
			cout << "  Planet creation finished, now name your planet! \n"
				 << "  Availiable names are all but these: \n";
			printPresets();
			cin >> input;
			presetMap.insert(make_pair(input, new Object(model, shader, shaderParams, texture1, texture2, 0,normals)));
			cout << "  New preset planet successfully created!\n";
			return;
		}
		
	}
	while (inPlanetCreator);
	return;
}

void Editor::setNewObject(Object* newObject, Object* currentObject)
{
	bool inObjectSettings = true;
	bool badInput = false;
	std::string input;
	float scale, density, distance;
	cv::Vec3f rotAngles, velocity;
	do
	{
		if (badInput == true)
			cout << "  Bad input, please start over\n";
		
		badInput = false;
		cout << "  Welcome to the object settings editor\n"
			 << "  First, specify desired rotations (around own axis) as: \"ax ay az\"\n";

		cin >> input;
		if (input == "EXIT")
			inObjectSettings = false;
		else
		{
			getline(cin, input);
			if(!(cin >> rotAngles[0]))
				badInput = true;
			if(!(cin >> rotAngles[1]))
				badInput = true;
			if(!(cin >> rotAngles[2]))
				badInput = true;
		}
		

		if (!badInput)
		{
			cout << "  Input successful, now specify rotation axis and speed for the object in orbit.\n"
				 << "  Orbit speed is given as the norm of the specified rotation axis; \"vx vy vz\"\n";
			getline(cin, input);
			if(!(cin >> velocity[0]))
				badInput = true;
			if(!(cin >> velocity[1]))
				badInput = true;
			if(!(cin >> velocity[2]))
				badInput = true;
		}
		if(!badInput)
		{
			cout << "  Input successful, now finally specify size, density and\n"
				 << "  distance to orbital center as: \"size density distance\"\n";
			getline(cin, input);
			if(!(cin >> scale))
				badInput = true;
			if(!(cin >> density))
				badInput = true;
			if(!(cin >> distance))
				badInput = true;
		}
		if(!badInput)
		{
			newObject->set(cv::Vec3f(0,0,10), cv::Vec3f(scale,scale,scale), rotAngles, velocity, density);
			newObject->distance = distance;
			currentObject->addSatellite(newObject, distance);
			cout << "  Placement successful!\n" << endl;
			inObjectSettings = false;
		}	
		
	}
	while (inObjectSettings);
	
	
	return;
}


void Editor::printHelp()
{
	cout << "  Avaliable commands are:\n \n"
		 << "  START				Start the simulator with default solar system\n"
		 << "  HELP				Print this information\n"
		 << "  RESET				Reset to default solar system\n"
		 << "  DELETE			Delete selected planet and satellites\n"
		 << "  DELETEALL			Remove all stars and planets\n"
		 << "  DELETEITEMS			Reset to default solar system\n"
		 << "  PLANETEDITOR			Enter planet editor\n"
	     << "  PLANETCREATOR			Create new preset planet\n"
		 << "  TEXTURES			Show all avaliable textures\n"
         << "  SHADERS			Show all shaders\n"
         << "  NORMALS			Show all normal maps\n"
         << "  PRESETS			Show all preset planets\n"
         << "  EXIT				Exit program\n";
}

void Editor::printWelcome()
{
	cout <<  "  \n\n"
		 <<  "  Welcome to CMS, to start with default Solar System, type \"START\",\n"
		 <<	 "  to enter the editor from within the simulator press the \"E\"-key \n"
		 <<  "  to select a planet for editing, right klick it.";
}

void Editor::printTextures()
{
	cout << "  Avaliable Textures are:\n \n";
	for(std::map<std::string, GLuint>::iterator it = textureMap.begin(); it != textureMap.end(); ++it)
	{
		cout << "  " << it->first << "\n";
	}
}

void Editor::printNormals()
{
	cout << "  Avaliable normal maps are:\n \n";
	for(std::map<std::string, GLuint>::iterator it = normalMap.begin(); it != normalMap.end(); ++it)
	{
		cout << "  " << it->first << "\n";
	}
}

void Editor::printShaders()
{
	cout << "  Avaliable shader programs are:\n \n";
	for(std::map<std::string, GLuint>::iterator it = shaderMap.begin(); it != shaderMap.end(); ++it)
	{
		cout << "  " << it->first << "\n";
	}
}

void Editor::printPresets()
{
	cout << "  Avaliable preset planet and stars are:\n \n";
	for(std::map<std::string, Object*>::iterator it = presetMap.begin(); it != presetMap.end(); ++it)
	{
		cout << "  " << it->first << "\n";
	}
}


void Editor::initDefaultSystem(SolarSystem& solsystem)
{
	Object* sun = presetMap["Sun"]->clone();
	Object* earth = presetMap["Earth"]->clone();
	Object* moon = presetMap["Moon"]->clone();
	Object* mercury = presetMap["Mercury"]->clone();
	Object* venus = presetMap["Venus"]->clone();
	Object* mars = presetMap["Mars"]->clone();
	Object* jupiter = presetMap["Jupiter"]->clone();
	Object* saturn = presetMap["Saturn"]->clone();
	Object* uranus = presetMap["Uranus"]->clone();
	Object* neptune = presetMap["Neptune"]->clone();
	Object* pluto = presetMap["Pluto"]->clone();
	
	sun->set(cv::Vec3f(0,0,0),  cv::Vec3f(10,10,10), cv::Vec3f(0,0,0), cv::Vec3f(0,0,0), 0.25);
	earth->set(cv::Vec3f(0,0,32),  cv::Vec3f(2,2,2), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	moon->set(cv::Vec3f(0,0,27),  cv::Vec3f(0.75,0.75,0.75), cv::Vec3f(0,0,0), cv::Vec3f(0,-10,0), 1);
	mercury->set(cv::Vec3f(0,0,15),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	venus->set(cv::Vec3f(0,0,22),  cv::Vec3f(1.9,1.9,1.9), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	mars->set(cv::Vec3f(0,0,45),  cv::Vec3f(1.75,1.75,1.75), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	jupiter->set(cv::Vec3f(0,0,65),  cv::Vec3f(5,5,5), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	saturn->set(cv::Vec3f(0,0,80),  cv::Vec3f(4,4,4), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	uranus->set(cv::Vec3f(0,0,95),  cv::Vec3f(3,3,3), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	neptune->set(cv::Vec3f(0,0,115),  cv::Vec3f(3,3,3), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	pluto->set(cv::Vec3f(0,0,130),  cv::Vec3f(1,1,1), cv::Vec3f(0,0,0), cv::Vec3f(0,10,0), 1);
	
	earth->addSatellite(moon, 5);
	sun->addSatellite(earth, 32);
	sun->addSatellite(mercury, 15);
	sun->addSatellite(venus, 22);
	sun->addSatellite(mars, 45);
	sun->addSatellite(jupiter, 65);
	sun->addSatellite(saturn, 80);
	sun->addSatellite(uranus, 95);
	sun->addSatellite(neptune, 115);
	sun->addSatellite(pluto, 130);

	solsystem.addStar(sun, true, cv::Vec3f(1,1,1));
}
// myUtils.h, Usefull help functions

#include "SolarSystem.h"
#include "Player.h"
#include "Model.h"
#include <random>

void handleEvents(sf::Window* window, bool* running, bool* editor, bool* selectObject, bool* cooldown, int* item, Player* player, float dt);
void generateSphere(Model* model, int subdivisions);
void bumpMySphere(Model* model, cv::Mat* bumpMap);
void drawSkybox(Player *player, Model* model, GLuint program, GLuint texture);

Object* getSelectedObject(std::list<Object*>* allObjects, Player* player);
/*
void edit(SolarSystem& solsystem,
		  std::map<std::string, Model*>& modelMap,
		  std::map<std::string, GLuint>& shaderMap,
		  std::map<std::string, GLuint>& textureMap,
		  std::map<std::string, GLuint>& normalMap);
		  */
/* maybe later
void getTexture(std::string textureName);
void getNormal(std::string normalName);
void getModel(std::string modelName);
void getProgram(std::string programName);
*/
void loadNoise();

extern GLfloat projectionMatrix[];
extern GLuint noiseTexture;
extern std::mt19937 generator;
extern std::normal_distribution<double> normalDistribution;
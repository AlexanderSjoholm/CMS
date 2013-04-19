// myUtils.h, Usefull help functions

#include "Player.h"
#include "Model.h"
#include <random>

void handleEvents(sf::Window* window, bool* running, int* item, Player* player, float dt);
void generateSphere(Model* model, int subdivisions);
void bumpMySphere(Model* model, cv::Mat* bumpMap);
void loadNoise();

extern GLfloat projectionMatrix[];
extern GLuint noiseTexture;
extern std::mt19937 generator;
extern std::normal_distribution<double> normalDistribution;
// myUtils.h, Usefull help functions

#include "Player.h"
#include "Model.h"

void handleEvents(sf::Window* window, bool* running, Player* player, float dt);
void generateSphere(Model* model, int subdivisions);
void bumpMySphere(Model* model, cv::Mat* bumpMap);

extern GLfloat projectionMatrix[];
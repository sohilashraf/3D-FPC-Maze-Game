#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "gl/glm/glm.hpp"
#include <gl/glm/gtc/type_ptr.hpp>
#include <gl/glm/gtx/transform.hpp>

using namespace std;
using namespace sf;
using namespace glm;

extern bool GameWon;

extern const int MAZE_WIDTH;
extern const int MAZE_HEIGHT;
extern int maze[10][10]; 

bool CheckCollisionAtXZ(float x, float z, float radius);
vec3 MoveWithCollision(vec3 currentPos, vec3 desiredPos, float radius);

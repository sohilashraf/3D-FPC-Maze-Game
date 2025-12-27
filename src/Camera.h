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

extern vec3 CameraPos;
extern vec3 CameraFront;
extern vec3 CameraUp;

extern const float CAMERA_HEIGHT;
extern float CameraSpeed;
extern float MouseSensitivity;
extern float Yaw;
extern float Pitch;

void UpdateViewMatrix();
void UpdateCameraDirection();

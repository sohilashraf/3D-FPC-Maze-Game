#include "Camera.h"
#include "Game.h" 

vec3 CameraPos = vec3(1.5f, 0.5f, 7.5f);
vec3 CameraFront = vec3(0.0f, 0.0f, 0.0f);
vec3 CameraUp = vec3(0.0f, 1.0f, 0.0f);

const float CAMERA_HEIGHT = 0.5f;
float CameraSpeed = 0.002f;
float MouseSensitivity = 0.18f;
float Yaw = 0.0f;
float Pitch = 0.0f;

void UpdateViewMatrix()
{
    vec3 CameraTarget = CameraPos + CameraFront;
    mat4 ViewMat = lookAt(CameraPos, CameraTarget, CameraUp);
    glUniformMatrix4fv(ViewMat_Location, 1, GL_FALSE, (GLfloat*)&ViewMat);
}

void UpdateCameraDirection()
{
    vec3 front;
    front.x = cos(radians(Yaw)) * cos(radians(Pitch));
    front.y = sin(radians(Pitch));
    front.z = sin(radians(Yaw)) * cos(radians(Pitch));
    CameraFront = normalize(front);
    UpdateViewMatrix();
}

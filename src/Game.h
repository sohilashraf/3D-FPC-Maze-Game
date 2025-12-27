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

//texture coordinates
struct VERTEX
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

extern const GLint WIDTH, HEIGHT;
extern GLuint ProgramId;
extern GLuint WallTexture, FloorTexture, GoalTexture;

// VAO, VBO, and EBO
extern GLuint VAO_Wall, VBO_Wall, EBO_Wall;
extern GLuint VAO_Floor, VBO_Floor, EBO_Floor;
extern GLuint VAO_Goal, VBO_Goal, EBO_Goal;

// Uniform locations
extern GLuint ModelMat_Location, ViewMat_Location, ProjMat_Location;
extern GLuint UseTexture_Location;
extern GLuint LightPos_Location, ViewPos_Location, LightColor_Location;

// Goal rotation angle
extern float GoalRotation;

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& ProgramId);
GLuint LoadTexture(const char* filename);

void CreateWall();
void CreateFloor();
void CreateGoal();

int Init();
void Update();
void Render();

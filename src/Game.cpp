#include "Game.h"
#include "Camera.h"
#include "Maze.h"

const GLint WIDTH = 1280, HEIGHT = 720;

GLuint ProgramId;
GLuint WallTexture, FloorTexture, GoalTexture;

GLuint VAO_Wall, VBO_Wall, EBO_Wall;
GLuint VAO_Floor, VBO_Floor, EBO_Floor;
GLuint VAO_Goal, VBO_Goal, EBO_Goal;

GLuint ModelMat_Location, ViewMat_Location, ProjMat_Location;
GLuint UseTexture_Location;
GLuint LightPos_Location, ViewPos_Location, LightColor_Location;

float GoalRotation = 0.0f;

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& ProgramId)
{
    ProgramId = InitShader(vertex_shader_file_name, fragment_shader_file_name);
    glUseProgram(ProgramId);
}

GLuint LoadTexture(const char* filename)
{
    Image image;
    if (!image.loadFromFile(filename))
    {
        cout << "Failed to load texture: " << filename << endl;
        return 0;
    }

    image.flipVertically();

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

void CreateWall()
{
    VERTEX wallVertices[] =
    {
        { vec3(-0.5f, -0.5f,  0.5f), vec3(0, 0, 1), vec2(0, 0) },
        { vec3(0.5f, -0.5f,  0.5f), vec3(0, 0, 1), vec2(1, 0) },
        { vec3(0.5f,  0.5f,  0.5f), vec3(0, 0, 1), vec2(1, 1) },
        { vec3(-0.5f,  0.5f,  0.5f), vec3(0, 0, 1), vec2(0, 1) },

        { vec3(0.5f, -0.5f, -0.5f), vec3(0, 0, -1), vec2(0, 0) },
        { vec3(-0.5f, -0.5f, -0.5f), vec3(0, 0, -1), vec2(1, 0) },
        { vec3(-0.5f,  0.5f, -0.5f), vec3(0, 0, -1), vec2(1, 1) },
        { vec3(0.5f,  0.5f, -0.5f), vec3(0, 0, -1), vec2(0, 1) },

        { vec3(-0.5f, -0.5f, -0.5f), vec3(-1, 0, 0), vec2(0, 0) },
        { vec3(-0.5f, -0.5f,  0.5f), vec3(-1, 0, 0), vec2(1, 0) },
        { vec3(-0.5f,  0.5f,  0.5f), vec3(-1, 0, 0), vec2(1, 1) },
        { vec3(-0.5f,  0.5f, -0.5f), vec3(-1, 0, 0), vec2(0, 1) },

        { vec3(0.5f, -0.5f,  0.5f), vec3(1, 0, 0), vec2(0, 0) },
        { vec3(0.5f, -0.5f, -0.5f), vec3(1, 0, 0), vec2(1, 0) },
        { vec3(0.5f,  0.5f, -0.5f), vec3(1, 0, 0), vec2(1, 1) },
        { vec3(0.5f,  0.5f,  0.5f), vec3(1, 0, 0), vec2(0, 1) },

        { vec3(-0.5f,  0.5f,  0.5f), vec3(0, 1, 0), vec2(0, 0) },
        { vec3(0.5f,  0.5f,  0.5f), vec3(0, 1, 0), vec2(1, 0) },
        { vec3(0.5f,  0.5f, -0.5f), vec3(0, 1, 0), vec2(1, 1) },
        { vec3(-0.5f,  0.5f, -0.5f), vec3(0, 1, 0), vec2(0, 1) },

        { vec3(-0.5f, -0.5f, -0.5f), vec3(0, -1, 0), vec2(0, 0) },
        { vec3(0.5f, -0.5f, -0.5f), vec3(0, -1, 0), vec2(1, 0) },
        { vec3(0.5f, -0.5f,  0.5f), vec3(0, -1, 0), vec2(1, 1) },
        { vec3(-0.5f, -0.5f,  0.5f), vec3(0, -1, 0), vec2(0, 1) }
    };

    GLuint wallIndices[] =
    {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    glGenVertexArrays(1, &VAO_Wall);
    glGenBuffers(1, &VBO_Wall);
    glGenBuffers(1, &EBO_Wall);

    glBindVertexArray(VAO_Wall);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Wall);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Wall);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wallIndices), wallIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, TexCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void CreateFloor()
{
    VERTEX floorVertices[] =
    {
        { vec3(-0.5f, 0.0f, -0.5f), vec3(0, 1, 0), vec2(0, 0) },
        { vec3(0.5f, 0.0f, -0.5f), vec3(0, 1, 0), vec2(1, 0) },
        { vec3(0.5f, 0.0f,  0.5f), vec3(0, 1, 0), vec2(1, 1) },
        { vec3(-0.5f, 0.0f,  0.5f), vec3(0, 1, 0), vec2(0, 1) }
    };

    GLuint floorIndices[] = { 0, 2, 1, 0, 3, 2 };

    glGenVertexArrays(1, &VAO_Floor);
    glGenBuffers(1, &VBO_Floor);
    glGenBuffers(1, &EBO_Floor);

    glBindVertexArray(VAO_Floor);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Floor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Floor);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, TexCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void CreateGoal()
{
    float size = 0.2f;

    VERTEX goalVertices[] =
    {
        { vec3(0.0f, size, 0.0f), vec3(0, 1, 0), vec2(0.5f, 0.5f) },
        { vec3(-size, 0.0f, -size), vec3(-1, 0, -1), vec2(0, 0) },
        { vec3(size, 0.0f, -size), vec3(1, 0, -1), vec2(1, 0) },
        { vec3(size, 0.0f, size), vec3(1, 0, 1), vec2(1, 1) },
        { vec3(-size, 0.0f, size), vec3(-1, 0, 1), vec2(0, 1) },
        { vec3(0.0f, -size, 0.0f), vec3(0, -1, 0), vec2(0.5f, 0.5f) }
    };

    GLuint goalIndices[] =
    {
        0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1,
        5, 2, 1, 5, 3, 2, 5, 4, 3, 5, 1, 4
    };

    glGenVertexArrays(1, &VAO_Goal);
    glGenBuffers(1, &VBO_Goal);
    glGenBuffers(1, &EBO_Goal);

    glBindVertexArray(VAO_Goal);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Goal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(goalVertices), goalVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Goal);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(goalIndices), goalIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, Normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)offsetof(VERTEX, TexCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

int Init()
{
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "Error initializing GLEW" << endl;
        return 1;
    }

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    CompileShader("shaders/VS.glsl", "shaders/FS.glsl", ProgramId);

    WallTexture = LoadTexture("assets/textures/wall.png");
    FloorTexture = LoadTexture("assets/textures/floor.jpg");
    GoalTexture = LoadTexture("assets/textures/goal.jpg");

    cout << "Goal Texture ID: " << GoalTexture << endl;
    if (GoalTexture == 0) cout << "ERROR: Goal texture failed to load!\n";


    if (WallTexture == 0) cout << "ERROR: Wall texture failed to load!" << endl;
    else cout << "Wall texture loaded successfully!" << endl;

    if (FloorTexture == 0) cout << "ERROR: Floor texture failed to load!" << endl;
    else cout << "Floor texture loaded successfully!" << endl;

    CreateFloor();
    CreateWall();
    CreateGoal();

    ModelMat_Location = glGetUniformLocation(ProgramId, "model_mat");
    ViewMat_Location = glGetUniformLocation(ProgramId, "view_mat");
    ProjMat_Location = glGetUniformLocation(ProgramId, "proj_mat");
    UseTexture_Location = glGetUniformLocation(ProgramId, "useTexture");
    LightPos_Location = glGetUniformLocation(ProgramId, "lightPos");
    ViewPos_Location = glGetUniformLocation(ProgramId, "viewPos");
    LightColor_Location = glGetUniformLocation(ProgramId, "lightColor");

    GLuint textureSamplerLocation = glGetUniformLocation(ProgramId, "textureSampler");
    glUniform1i(textureSamplerLocation, 0);

    UpdateCameraDirection();

    mat4 ProjMat_Perspective = perspectiveFov(70.0f, (float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(ProjMat_Location, 1, GL_FALSE, (GLfloat*)&ProjMat_Perspective);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return 0;
}

void Update()
{
    const float radius = 0.20f;

    vec3 forward = normalize(vec3(CameraFront.x, 0.0f, CameraFront.z));
    vec3 right = normalize(cross(forward, CameraUp));

    vec3 desired = CameraPos;

    if (Keyboard::isKeyPressed(Keyboard::W)) desired += forward * CameraSpeed;
    if (Keyboard::isKeyPressed(Keyboard::S)) desired -= forward * CameraSpeed;
    if (Keyboard::isKeyPressed(Keyboard::A)) desired -= right * CameraSpeed;
    if (Keyboard::isKeyPressed(Keyboard::D)) desired += right * CameraSpeed;

    desired.y = CAMERA_HEIGHT;

    CameraPos = MoveWithCollision(CameraPos, desired, radius);
    UpdateViewMatrix();

    GoalRotation += 4.0f;
}

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform3f(LightPos_Location, 5.0f, 5.0f, 5.0f);
    glUniform3f(ViewPos_Location, CameraPos.x, CameraPos.y, CameraPos.z);
    glUniform3f(LightColor_Location, 1.0f, 1.0f, 1.0f);

    for (int z = 0; z < MAZE_HEIGHT; z++)
    {
        for (int x = 0; x < MAZE_WIDTH; x++)
        {
            if (maze[z][x] == 1)
            {
                mat4 WallMat = translate(vec3(x + 0.5f, 0.5f, z + 0.5f));

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, WallTexture);
                glUniform1i(UseTexture_Location, 1);
                glBindVertexArray(VAO_Wall);
                glUniformMatrix4fv(ModelMat_Location, 1, GL_FALSE, (GLfloat*)&WallMat);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
            else if (maze[z][x] == 0)
            {
                mat4 FloorMat = translate(vec3(x + 0.5f, 0.0f, z + 0.5f));

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, FloorTexture);
                glUniform1i(UseTexture_Location, 1);
                glBindVertexArray(VAO_Floor);
                glUniformMatrix4fv(ModelMat_Location, 1, GL_FALSE, (GLfloat*)&FloorMat);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
            else if (maze[z][x] == 2)
            {
                mat4 FloorMat = translate(vec3(x + 0.5f, 0.0f, z + 0.5f));

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, FloorTexture);
                glUniform1i(UseTexture_Location, 1);
                glBindVertexArray(VAO_Floor);
                glUniformMatrix4fv(ModelMat_Location, 1, GL_FALSE, (GLfloat*)&FloorMat);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                mat4 GoalMat = translate(vec3(x + 0.5f, 0.4f, z + 0.5f)) *
                    rotate(radians(GoalRotation), vec3(0, 1, 0));

                glDisable(GL_CULL_FACE);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, GoalTexture);
                glUniform1i(UseTexture_Location, 1);
                glBindVertexArray(VAO_Goal);
                glUniformMatrix4fv(ModelMat_Location, 1, GL_FALSE, (GLfloat*)&GoalMat);
                glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
                glEnable(GL_CULL_FACE);
            }
        }
    }
}

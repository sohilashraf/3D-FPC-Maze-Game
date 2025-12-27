#include "Maze.h"
#include "Camera.h" 

bool GameWon = false;

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 10;

int maze[MAZE_HEIGHT][MAZE_WIDTH] =
{
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,2,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

bool CheckCollisionAtXZ(float x, float z, float radius)
{
    int gridX = (int)floor(x);
    int gridZ = (int)floor(z);

    if (gridX < 0 || gridX >= MAZE_WIDTH || gridZ < 0 || gridZ >= MAZE_HEIGHT)
        return true;

    for (int dz = -1; dz <= 1; dz++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            int checkX = gridX + dx;
            int checkZ = gridZ + dz;

            if (checkX < 0 || checkX >= MAZE_WIDTH || checkZ < 0 || checkZ >= MAZE_HEIGHT)
                continue;

            if (maze[checkZ][checkX] == 1)
            {
                float wallMinX = (float)checkX;
                float wallMaxX = (float)checkX + 1.0f;
                float wallMinZ = (float)checkZ;
                float wallMaxZ = (float)checkZ + 1.0f;

                float closestX = glm::clamp(x, wallMinX, wallMaxX);
                float closestZ = glm::clamp(z, wallMinZ, wallMaxZ);

                float distX = x - closestX;
                float distZ = z - closestZ;

                if ((distX * distX + distZ * distZ) < (radius * radius))
                    return true;
            }
        }
    }

    if (maze[gridZ][gridX] == 2)
    {
        GameWon = true;
        cout << "\n*** CONGRATULATIONS! ***\n" << endl;
    }

    return false;
}

vec3 MoveWithCollision(vec3 currentPos, vec3 desiredPos, float radius)
{
    vec3 result = currentPos;

    float testX = desiredPos.x;
    if (!CheckCollisionAtXZ(testX, result.z, radius))
        result.x = testX;

    float testZ = desiredPos.z;
    if (!CheckCollisionAtXZ(result.x, testZ, radius))
        result.z = testZ;

    result.y = CAMERA_HEIGHT;
    return result;
}

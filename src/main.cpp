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

#include "Game.h"
#include "Camera.h"
#include "Maze.h"

int main()
{
    Window window(VideoMode(WIDTH, HEIGHT), "3D Maze Explorer", Style::Close, ContextSettings(24));

    if (Init())
        return 1;

    window.setMouseCursorVisible(false);
    Vector2i windowCenter(WIDTH / 2, HEIGHT / 2);
    Mouse::setPosition(windowCenter, window);

    vec2 lastMousePos(WIDTH / 2.0f, HEIGHT / 2.0f);
    bool firstMouse = true;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                window.close();
        }

        Vector2i currentMousePos = Mouse::getPosition(window);

        if (firstMouse)
        {
            lastMousePos = vec2(currentMousePos.x, currentMousePos.y);
            firstMouse = false;
        }
        else
        {
            vec2 currentMouse(currentMousePos.x, currentMousePos.y);
            vec2 mouseDelta = currentMouse - lastMousePos;

            if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
            {
                Yaw += mouseDelta.x * MouseSensitivity;
                Pitch = clamp(Pitch - mouseDelta.y * MouseSensitivity, -89.0f, 89.0f);
                UpdateCameraDirection();
            }
        }

        Mouse::setPosition(windowCenter, window);
        lastMousePos = vec2(WIDTH / 2.0f, HEIGHT / 2.0f);

        if (!GameWon)
            Update();

        Render();
        window.display();
    }

    return 0;
}

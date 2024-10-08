#include "input.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace minigl
{
    bool Input::isKeyPressed(int keyCode) const
    {
        auto state = glfwGetKey(window, keyCode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(int button) const
    {
        auto state = glfwGetMouseButton(window, button);

        return state == GLFW_PRESS;
    }

    float Input::getMouseX() const
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return (float)xPos;
    }

    float Input::getMouseY() const
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return (float)yPos;
    }

    Vec2 Input::getMousePos() const
    {
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return {(float)xPos, (float)yPos};
    }

    Vec2 Input::getMouseScreenPos() const
    {
        double xPos, yPos;
        int xWindowPos, yWindowPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        glfwGetWindowPos(window, &xWindowPos, &yWindowPos);

        return {(float)(xWindowPos + xPos), (float)(yWindowPos + yPos)};
    }

    bool Input::mouseInArea(const Vec2& p0, const Vec2& p1, bool screenCoords) const
    {
        auto mousePos = screenCoords ? getMouseScreenPos() : getMousePos();
        return (mousePos.x > p0.x && mousePos.x < p1.x && mousePos.y > p0.y && mousePos.y < p1.y);
    }
}
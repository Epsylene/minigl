#include "camera.hpp"

#include "input/input.hpp"
#include "input/key_codes.h"
#include "input/mouse_codes.h"

namespace minigl
{
    Camera::Camera(): Camera(90.f, 16.f/9.f, 0.3f, 10.f)
    {}

    Camera::Camera(float fov, float aspect, float near, float far):
        pos(0.f, 0.f, 1.f), direction(0.f, 0.f, -1.f), up(0.f, 1.f, 0.f)
    {
        proj = perspective(fov, aspect, near, far);
        recalculateViewProj();
    }

    void Camera::setPosition(const Vec3& pos)
    {
        this->pos = pos;
        recalculateViewProj();
    }

    const Vec3& Camera::getPosition() const
    {
        return pos;
    }

    const Vec3& Camera::getDirection() const
    {
        return direction;
    }

    void Camera::set_rotation(float pitch, float yaw, float roll)
    {
        this->pitch = pitch;
        this->yaw = yaw;
        this->roll = roll;
        recalculateViewProj();
    }

    void Camera::change_projection(float fov, float aspect, float near, float far)
    {
        proj = perspective(fov, aspect, near, far);
        recalculateViewProj();
    }

    void Camera::recalculateViewProj()
    {
        // The camera direction
        direction = { std::cos(radians(yaw)) * std::cos(radians(pitch)),
                      std::sin(radians(pitch)),
                      std::sin(radians(yaw)) * std::cos(radians(pitch)) };
        direction = normalize(direction);

        // The camera's up vector
        up = rotate({0.f, 1.f, 0.f}, direction, roll);

        // The lookAt() function builds a view matrix that
        // looks from a position (pos) to a certain point (pos
        // + direction) in a space whose up-pointing vector is
        // 'up'.
        view = lookAt(pos, pos + direction, up);
        viewProj = proj * view;
    }

    void Camera::onUpdate(Ref<Input> input, float dt)
    {
        Vec3 dir = direction;
        Vec3 up {0, 1, 0};

        float movSpeed = 5.f;

        // Move around
        if(input->isKeyPressed(GLS_KEY_W))
            pos += dir * movSpeed * float(dt);
        if(input->isKeyPressed(GLS_KEY_A))
            pos -= normalize(cross(dir, up)) * movSpeed * float(dt);
        if(input->isKeyPressed(GLS_KEY_S))
            pos -= dir * movSpeed * float(dt);
        if(input->isKeyPressed(GLS_KEY_D))
            pos += normalize(cross(dir, up)) * movSpeed * float(dt);

        // Move up and down
        if(input->isKeyPressed(GLS_KEY_R))
            pos.y += movSpeed * dt;
        if(input->isKeyPressed(GLS_KEY_F))
            pos.y -= movSpeed * dt;

        // Look around with the mouse
        if(input->isMouseButtonPressed(GLS_MOUSE_BUTTON_LEFT))
        {
            float xOffset = input->getMouseX() - input->lastMousePos.x;
            float yOffset = input->lastMousePos.y - input->getMouseY();

            pitch += yOffset * 200 * dt;
            yaw += xOffset * 200 * dt;

            // pitch = glm::clamp(pitch, -89.f, 89.f);
            set_rotation(pitch, yaw, roll);
        }
        input->lastMousePos = input->getMousePos();

        setPosition(pos);
    }
}
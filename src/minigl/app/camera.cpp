#include "camera.hpp"

#include "input/key_codes.h"
#include "input/mouse_codes.h"

namespace minigl
{
    Camera::Camera(): pos(0.f, 0.f, 3.f), direction(0.f, 0.f, -1.f), up(0.f, 1.f, 0.f)
    {}

    void Camera::setPosition(const Vec3& pos)
    {
        this->pos = pos;
        compute_viewProj();
    }

    void Camera::setDirection(const Vec3& dir)
    {
        direction = dir;
        compute_viewProj();
    }

    OrthoCamera::OrthoCamera(): OrthoCamera(-5.f, 5.f, -5.f, 5.f, 0.1f, 100.f)
    {}

    OrthoCamera::OrthoCamera(float left, float right, float bottom, float top, float near, float far)
    {
        this->left = left;
        this->right = right;
        this->bottom = bottom;
        this->top = top;
        this->near = near;
        this->far = far;

        proj = ortho(left, right, bottom, top, near, far);
    }

    void OrthoCamera::compute_viewProj()
    {
        view = lookAt(pos, pos + direction, up);
        viewProj = proj * view;
    }

    PerspectiveCamera::PerspectiveCamera(): PerspectiveCamera(60.f, 16.f/9.f, 0.3f, 100.f)
    {}

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    {
        proj = perspective(radians(fov), aspect, near, far);
    }

    void PerspectiveCamera::change_projection(float fov, float aspect, float near, float far)
    {
        proj = perspective(radians(fov), aspect, near, far);
        compute_viewProj();
    }

    void FreeCamera::set_rotation(float pitch, float yaw, float roll)
    {
        this->pitch = pitch;
        this->yaw = yaw;
        this->roll = roll;
        compute_viewProj();
    }

    void FreeCamera::compute_viewProj()
    {
        // The camera direction
        direction = { glm::cos(radians(yaw)) * glm::cos(radians(pitch)),
                      glm::sin(radians(pitch)),
                      glm::sin(radians(yaw)) * glm::cos(radians(pitch)) };
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

    void FreeCamera::onUpdate(Ref<Input> input, float dt)
    {
        Vec3 dir = direction;
        Vec3 up {0, 1, 0};

        bool shift = input->isKeyPressed(GLS_KEY_LEFT_SHIFT);
        float movSpeed = shift ? 0.1f : 5.f;
        float rotSpeed = shift ? 50.f : 200.f;

        movSpeed *= speed_factor;

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

            pitch += yOffset * rotSpeed * dt;
            yaw += xOffset * rotSpeed * dt;

            pitch = glm::clamp(pitch, -89.f, 89.f);
            set_rotation(pitch, yaw, roll);
        }
        input->lastMousePos = input->getMousePos();

        setPosition(pos);
    }

    void FreeCamera::setDirection(const Vec3& dir)
    {
        direction = dir;
        compute_yaw_pitch();
        compute_viewProj();
    }

    void FreeCamera::setSpeedFactor(float factor)
    {
        speed_factor = factor;
    }

    void FreeCamera::compute_yaw_pitch()
    {
        // Compute the yaw (rotation around the y-axis)
        yaw = degrees(atan2(direction.z, direction.x));

        // Compute the pitch (rotation around the x-axis)
        pitch = degrees(asin(direction.y));
    }
}
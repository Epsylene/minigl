#include "geometry.hpp"

namespace glose
{
    Mat4 translate(const Vec3& offset)
    {
        return glm::translate(Mat4{1.f}, offset);
    }

    Mat4 rotate(const Vec3& axis, float angle)
    {
        return glm::rotate(Mat4{1.f}, angle, axis);
    }

    Mat4 perspective(float fov, float aspect, float near, float far)
    {
        return glm::perspective(fov, aspect, near, far);
    }

    Mat4 scale(float factor)
    {
        return glm::scale(Mat4{1.f}, Vec3{factor});
    }

    Mat4 scale(const Vec3& factor)
    {
        return glm::scale(Mat4{1.f}, factor);
    }
}
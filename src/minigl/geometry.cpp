#include "geometry.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace minigl
{
    Mat4 translate(const Vec3& offset)
    {
        return glm::translate(Mat4{1.f}, offset);
    }

    Mat4 rotate(const Vec3& axis, float angle)
    {
        return glm::rotate(Mat4{1.f}, angle, axis);
    }

    Vec3 rotate(const Vec3& vec, const Vec3& axis, float angle)
    {
        return glm::rotate(vec, angle, axis);
    }

    Mat4 perspective(float fov, float aspect, float near, float far)
    {
        return glm::perspective(fov, aspect, near, far);
    }

    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up)
    {
        return glm::lookAt(eye, center, up);
    }

    Mat4 scale(float factor)
    {
        return glm::scale(Mat4{1.f}, Vec3{factor});
    }

    Mat4 scale(const Vec3& factor)
    {
        return glm::scale(Mat4{1.f}, factor);
    }

    float radians(float degrees)
    {
        return glm::radians(degrees);
    }

    Vec3 normalize(const Vec3& vec)
    {
        return glm::normalize(vec);
    }

    Vec3 cross(const Vec3& a, const Vec3& b)
    {
        return glm::cross(a, b);
    }

    float dot(const Vec3& a, const Vec3& b)
    {
        return glm::dot(a, b);
    }

    float length(const Vec3& vec)
    {
        return glm::length(vec);
    }
}
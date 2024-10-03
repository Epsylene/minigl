#pragma once

#include <glm/glm.hpp>

namespace minigl
{
    template<typename T, int N>
    using Vector = glm::vec<N, T>;

    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;

    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;

    constexpr double pi = 3.14159265358979323846;

    Mat4 translate(const Vec3& offset);
    Mat4 rotate(const Vec3& axis, float angle);
    Vec3 rotate(const Vec3& vec, const Vec3& axis, float angle);

    Mat4 perspective(float fov, float aspect, float near, float far);
    Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);

    Mat4 scale(float factor);
    Mat4 scale(const Vec3& factor);

    float radians(float degrees);
    float degrees(float radians);

    Vec3 normalize(const Vec3& vec);
    Vec3 cross(const Vec3& a, const Vec3& b);

    float dot(const Vec3& a, const Vec3& b);
    float length(const Vec3& vec);
}
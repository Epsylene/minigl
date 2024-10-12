#pragma once

#include <fmt/format.h>
#include <glm/glm.hpp>

namespace minigl
{
    template<typename T, int N>
    using Vector = glm::vec<N, T>;

    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;

    template<typename T, int N>
    using Matrix = glm::mat<N, N, T>;

    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;

    constexpr double pi = 3.14159265358979323846;

    Mat4 translate(const Vec3& offset);
    Mat4 rotate(const Vec3& axis, float angle);
    Vec3 rotate(const Vec3& vec, const Vec3& axis, float angle);

    Mat4 perspective(float fov, float aspect, float near, float far);
    Mat4 ortho(float left, float right, float bottom, float top, float near, float far);
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

// Vec3 formatting
template<typename T>
class fmt::formatter<minigl::Vector<T,3>>
{
    public:

        constexpr auto parse(format_parse_context& ctx)
        {
            return ctx.begin();
        }

        template<typename FormatContext>
        auto format(const minigl::Vector<T,3>& vec, FormatContext& ctx) const
        {
            return format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
        }
};
#pragma once
#include "geometry.hpp"

namespace minigl
{
    /// Color struct. This is actually the same as a Vec4 but
    /// with (r,g,b,a) components instead of (x,y,z,w).
    struct Color
    {
        float r, g, b, a;

        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Magenta;
        static const Color Brown;

        Color() = default;
        Color(float factor);

        /// Construct an RGBA color. The parameters are
        /// expected to range from 0 to 1, with the alpha set
        /// to 1 by default.
        Color(float r, float g, float b, float a = 1.f);

        Color(const Color& color) = default;

        /// Construct an RGB color (transparency set at 1)
        Color(const Vec3& color);
        Color(const Vec4& color);

        Color& operator+=(const Color& color);
        Color& operator-=(const Color& color);
        Color& operator*=(const Color& color);
        Color& operator*=(float factor);

        operator Vec3() const;
        operator Vec4() const;
    };

    Color operator+(const Color& c1, const Color& c2);
    Color operator-(const Color& c1, const Color& c2);
    Color operator*(const Color& c1, const Color& c2);
    Color operator*(const Color& color, float factor);
}